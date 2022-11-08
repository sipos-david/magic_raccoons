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