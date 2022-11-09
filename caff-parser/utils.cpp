#include "utils.hpp"

int bytesToInt(const std::vector<byte> &bytes) {
    int number = 0;

    for (std::size_t i = 0; i < bytes.size(); ++i) {
        number += bytes[i] * std::pow(16, i * 2);
    }

    return number;
}

std::vector<byte> *take(std::vector<byte> &from, const int elements) {
    auto *arr = new std::vector<byte>();
    int i = 0;
    for (auto it = from.begin(); it < from.end() && i < elements; it++) {
        ++i;
        arr->push_back(*it);
    }
    if (i != elements) {
        delete arr;
        return nullptr;
    }
    from.erase(from.begin(), from.begin() + elements);
    return arr;
}

int takeInt(std::vector<byte> &from, const int bytes) {
    const std::vector<byte> *rawData = take(from, bytes);
    if (rawData == nullptr) {
        throw EmptyByteVectorException();
    }
    const int data = bytesToInt(*rawData);
    delete rawData;
    return data;
}

std::vector<byte> *takeUntil(std::vector<byte> &from, byte until) {
    auto *arr = new std::vector<byte>();
    int i = 0;
    bool found = false;
    for (auto it = from.begin(); it < from.end() && !found; it++) {
        ++i;
        arr->push_back(*it);
        if (*it == until) {
            found = true;
        }
    }
    if (!found) {
        delete arr;
        return nullptr;
    }
    from.erase(from.begin(), from.begin() + i);
    return arr;
}

void printTGA(std::string path, int width, int height, const std::vector<Pixel> *pixels) {
    std::ofstream tga(path, std::ios::out | std::ios::binary);
    tga.write((char *) 0, sizeof(char)); // image ID
    tga.write((char *) 0, sizeof(char)); // color map type
    tga.write((char *) 2, sizeof(char)); // image type: uncompressed RGB

    // Color map specification
    tga.write((char *) 0, sizeof(short int));
    tga.write((char *) 0, sizeof(short int));
    tga.write((char *) 0, sizeof(char));

    // Image specification
    tga.write((char *) 0, sizeof(short int)); // x origin
    tga.write((char *) 0, sizeof(short int)); // y origin
    tga.write((char *) width, sizeof(short int)); // width
    tga.write((char *) height, sizeof(short int)); // height
    tga.write((char *) 24, sizeof(short int)); // 24 bit bitmap

    for (auto &pixel: *pixels) {
        tga.write((char *) pixel.getRed(), sizeof(char));
        tga.write((char *) pixel.getGreen(), sizeof(char));
        tga.write((char *) pixel.getBlue(), sizeof(char));
    }

    tga.close();
}