#include <iostream>
#include <vector>
#include "math.h"

using byte = unsigned char;

int bytesToInt(std::vector<byte> bytes)
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
    std::vector<byte> *arr = new std::vector<byte>();
    for (size_t i = 0; i < elements; i++)
    {
        arr->push_back(from[i]);
    }
    from.erase(from.begin(), from.begin() + elements);
    return arr;
}

int takeInt(std::vector<byte> &from, const int bytes = 8)
{
    const std::vector<byte> *rawData = take(from, 8);
    const int data = bytesToInt(*rawData);
    delete rawData;
    return data;
}