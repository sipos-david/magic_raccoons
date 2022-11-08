#include "caff.hpp"

std::vector <byte> loadFile(std::string const &filepath) {
    std::ifstream input(filepath, std::ios::binary);

    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}

int main(int argc, char const *argv[]) {
    // argv[1] => input file
    // argv[2] => output folder (preview.tga, metadata.json)
    if (argc != 3) {
        // args not provided
        return 1;
    }

    // TODO check file write permissions

    std::vector <byte> file = loadFile(argv[1]);

    CAFF::CaffHeaderResult hResult = CAFF::parseCaffHeader(file);

    if (hResult.getResult() != OK_RESULT) {
        std::cout << hResult.getResult();
        return 1;
    }
    std::cout << "----- CAFF:Header -----" << std::endl;
    std::cout << hResult.getHeaderSize() << std::endl;
    std::cout << hResult.getNumAnim() << std::endl;

    try {
        int nextBlockId = takeInt(file, 1);
        int blockLength = takeInt(file, 8);
        while (!file.empty()) {
            std::vector <byte> *block = take(file, blockLength);
            if (nextBlockId == 2) {
                CAFF::CaffCreditsResult cResult = CAFF::parseCredits(*block);
                std::cout << "----- CAFF:Credits -----" << std::endl;
                std::cout << cResult.getYear() << std::endl;
                std::cout << cResult.getDay() << std::endl;
                std::cout << cResult.getHour() << std::endl;
                std::cout << cResult.getMonth() << std::endl;
                std::cout << cResult.getCreatorLen() << std::endl;
                std::cout << cResult.getCreator() << std::endl;
                std::cout << cResult.getResult() << std::endl;
            } else if (nextBlockId == 3) {
                CAFF::CaffAnimationResult aResult = CAFF::parseAnimation(*block);
                std::cout << "----- CAFF:Animation -----" << std::endl;
                std::cout << aResult.getDuration() << std::endl;
                std::cout << aResult.getResult() << std::endl;
            } else {
                std::cout << "Unknown block id!" << std::endl;
            }
            nextBlockId = takeInt(file, 1);
            blockLength = takeInt(file, 8);
        }
    } catch (EmptyByteVectorException &e) {
        // TODO write error in json
    }

    // TODO write preview.tga, metadata.json

    return 0;
}
