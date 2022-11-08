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
    std::cout << "----- CAFF:Header -----" << std::endl;
    std::cout << hResult.getHeaderSize() << std::endl;
    std::cout << hResult.getNumAnim() << std::endl;

    const int nextBlockId = takeInt(file, 1);
    const int blockLength = takeInt(file, 8);
    if (nextBlockId == 2)
    {
        CAFF::CaffCreditsResult cResult = CAFF::parseCredits(file);
        std::cout << "----- CAFF:Credits -----" << std::endl;
        std::cout << cResult.getYear() << std::endl;
        std::cout << cResult.getDay() << std::endl;
        std::cout << cResult.getHour() << std::endl;
        std::cout << cResult.getMonth() << std::endl;
        std::cout << cResult.getCreatorLen() << std::endl;
        std::cout << cResult.getCreator() << std::endl;
        std::cout << cResult.getResult() << std::endl;

        const int blockId = takeInt(file, 1);
        const int blockLength = takeInt(file, 8);
        if (blockId != 3)
        {
            return 1;
        }
        CAFF::CaffAnimationResult aResult = CAFF::parseAnimation(file);
        std::cout << "----- CAFF:Animation -----" << std::endl;
        std::cout << aResult.getDuration() << std::endl;
        std::cout << aResult.getResult() << std::endl;
    }
    else
    {
        CAFF::parseAnimation(file);
        const int blockId = takeInt(file, 1);
        const int blockLength = takeInt(file, 8);
        if (blockId != 2)
        {
            return 1;
        }
        CAFF::parseCredits(file);
    }

    return 0;
}
