#include <string>
#include <fstream>

#include "caff.hpp"

std::vector<byte> loadFile(std::string const &filepath)
{
    std::ifstream input(filepath, std::ios::binary);

    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        // args not provided
        return 1;
    }

    std::vector<byte> file = loadFile(argv[1]);

    CAFF::CaffHeaderResult hResult = CAFF::parseCaffHeader(file);

    if (hResult.getResult() != OK_RESULT)
    {
        std::cout << hResult.getResult();
        return 1;
    }

    const int nextBlockId = takeInt(file, 1);
    if (nextBlockId == 2)
    {
        CAFF::parseCredits(file);
        const int blockId = takeInt(file, 1);
        if (blockId != 3)
        {
            return 1;
        }
        CAFF::parseAnimation(file);
    }
    else
    {
        CAFF::parseAnimation(file);
        const int blockId = takeInt(file, 1);
        if (blockId != 2)
        {
            return 1;
        }
        CAFF::parseCredits(file);
    }

    return 0;
}
