#include <string>
#include <ios>
#include <fstream>

#include "ciff.hpp" // to be changed to caff

std::vector<byte> loadFile(std::string const& filepath)
{
    std::ifstream input( filepath, std::ios::binary );

    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}

