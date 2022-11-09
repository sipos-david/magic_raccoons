#include "utils.hpp"

int bytesToInt(const std::vector<byte> &bytes)
{
    int number = 0;

    for (std::size_t i = 0; i < bytes.size(); ++i)
    {
        number += bytes[i] * std::pow(16, i * 2);
    }

    return number;
}

std::vector<byte> *take(std::vector<byte> &from, const int elements)
{
    auto *arr = new std::vector<byte>();
    int i = 0;
    for (auto it = from.begin(); it < from.end() && i < elements; it++)
    {
        ++i;
        arr->push_back(*it);
    }
    if (i != elements)
    {
        delete arr;
        return nullptr;
    }
    from.erase(from.begin(), from.begin() + elements);
    return arr;
}

int takeInt(std::vector<byte> &from, const int bytes)
{
    const std::vector<byte> *rawData = take(from, bytes);
    if (rawData == nullptr)
    {
        throw EmptyByteVectorException();
    }
    const int data = bytesToInt(*rawData);
    delete rawData;
    return data;
}

std::vector<byte> *takeUntil(std::vector<byte> &from, byte until)
{
    auto *arr = new std::vector<byte>();
    int i = 0;
    bool found = false;
    for (auto it = from.begin(); it < from.end() && !found; it++)
    {
        ++i;
        arr->push_back(*it);
        if (*it == until)
        {
            found = true;
        }
    }
    if (!found)
    {
        delete arr;
        return nullptr;
    }
    from.erase(from.begin(), from.begin() + i);
    return arr;
}

void printTGA(std::string path, int width, int height, const std::vector<Pixel> *pixels)
{
    std::ofstream tga(path, std::ios::binary);
    if (!tga)
        throw CantOpenFileException();

    char header[18] = {0};
    header[0] = 0;
    header[1] = 0;
    header[2] = 0;
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 24; // bits per pixel
    std::string s(header, sizeof(header));
    tga.write((const char *)header, 18);

    for (auto &pixel : *pixels)
    {
        std::string red = std::to_string(pixel.getRed());
        std::string green = std::to_string(pixel.getGreen());
        std::string blue = std::to_string(pixel.getBlue());
        tga.write(red.c_str(), sizeof(char));
        tga.write(green.c_str(), sizeof(char));
        tga.write(blue.c_str(), sizeof(char));
    }

    static const char footer[26] =
        "\0\0\0\0"         // no extension area
        "\0\0\0\0"         // no developer directory
        "TRUEVISION-XFILE" // yep, this is a TGA file
        ".";
    tga.write(footer, 26);

    tga.close();
}