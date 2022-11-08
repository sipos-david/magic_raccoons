#ifndef CAFF_PARSER_UTILS_H
#define CAFF_PARSER_UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using byte = unsigned char;

const std::string OK_RESULT = "ok";

int bytesToInt(const std::vector<byte> &bytes);

std::vector<byte> *take(std::vector<byte> &from, int elements);

int takeInt(std::vector<byte> &from, int bytes = 8);

#endif // CAFF_PARSER_UTILS_H
