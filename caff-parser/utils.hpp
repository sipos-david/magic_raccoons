#ifndef CAFF_PARSER_UTILS_H
#define CAFF_PARSER_UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <exception>

using byte = unsigned char;

struct EmptyByteVectorException : std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "CAFF::EmptyByteVectorException";
    }
};

const std::string OK_RESULT = "ok";

int bytesToInt(const std::vector<byte> &bytes);

std::vector<byte> *take(std::vector<byte> &from, int elements);

int takeInt(std::vector<byte> &from, int bytes = 8);

#endif // CAFF_PARSER_UTILS_H
