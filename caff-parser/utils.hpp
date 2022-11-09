#ifndef CAFF_PARSER_UTILS_H
#define CAFF_PARSER_UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <exception>

using byte = unsigned char;

class Pixel
{
public:
    int getRed() const { return red; }
    int getGreen() const { return green; }
    int getBlue() const { return blue; }
    Pixel(int _red, int _green, int _blue)
    {
        red = _red;
        green = _green;
        blue = _blue;
    }

private:
    int red;
    int green;
    int blue;
};
struct EmptyByteVectorException : std::exception
{
    [[nodiscard]] const char *what() const noexcept override
    {
        return "CAFF::EmptyByteVectorException";
    }
};

struct MultipleCreditsException : std::exception
{
    [[nodiscard]] const char *what() const noexcept override
    {
        return "CAFF::MultipleCreditsException";
    }
};

const std::string OK_RESULT = "ok";

int bytesToInt(const std::vector<byte> &bytes);

std::vector<byte> *take(std::vector<byte> &from, int elements);

int takeInt(std::vector<byte> &from, int bytes = 8);

std::vector<byte> *takeUntil(std::vector<byte> &from, byte until);

void printTGA(std::string path, int width, int height, const std::vector<Pixel> *pixels);

#endif // CAFF_PARSER_UTILS_H
