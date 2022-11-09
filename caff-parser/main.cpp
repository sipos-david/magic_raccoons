#include "caff.hpp"
#include <regex>

std::vector<byte> loadFile(std::string const &filepath) {
    std::ifstream input(filepath, std::ios::binary);

    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}

void writeJsonCredits(std::ofstream &outFile, CAFF::CaffCreditsResult *cResult) {
    outFile << "{" << std::endl;
    outFile << "\t\"credits\":" << std::endl;
    outFile << "\t{" << std::endl;
    outFile << "\t\t\"year\":";
    outFile << " " << cResult->getYear() << "," << std::endl;
    outFile << "\t\t\"day\":";
    outFile << " " << cResult->getDay() << "," << std::endl;
    outFile << "\t\t\"hour\":";
    outFile << " " << cResult->getHour() << "," << std::endl;
    outFile << "\t\t\"month\":";
    outFile << " " << cResult->getMonth() << "," << std::endl;
    outFile << "\t\t\"creator\":";
    outFile << " \"" << cResult->getCreator() << "\"" << std::endl;
    outFile << "\t}," << std::endl;
}

void writeJsonAnimation(std::ofstream &outFile, std::vector<CAFF::CaffAnimationResult *> aResult) {
    outFile << "\t\"animation\":" << std::endl;
    outFile << "\t[" << std::endl;
    for (int i = 0; i < aResult.size(); i++) {
        outFile << "\t\t{" << std::endl;
        outFile << "\t\t\"duration\":";
        outFile << " " << aResult[i]->getDuration() << std::endl;
        if (i == aResult.size() - 1) {
            outFile << "\t\t}" << std::endl;
        } else {
            outFile << "\t\t}," << std::endl;
        }
    }
    outFile << "\t]" << std::endl;
    outFile << "}" << std::endl;
    outFile.close();
}

int main(int argc, char const *argv[]) {
    // argv[1] => input file
    // argv[2] => output folder (preview.tga, metadata.json)

    if (argc != 3) {
        // args not provided
        return 1;
    }

    // TODO check file write permissions

    std::string filename = argv[2];
    std::regex file_regex("^(?!.*_).*\.json$");

    if (!regex_match(filename, file_regex)) {
        std::cout << "You must provide a txt file!" << std::endl;
        return 1;
    }

    std::ofstream outFile(filename);

    std::vector<byte> file = loadFile(argv[1]);

    CAFF::CaffHeaderResult hResult = CAFF::parseCaffHeader(file);
    CAFF::CaffCreditsResult *cResult = nullptr;
    std::vector<CAFF::CaffAnimationResult *> animations;

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
            std::vector<byte> *block = take(file, blockLength);
            if (nextBlockId == 2) {
                if (cResult != nullptr) {
                    throw MultipleCreditsException();
                }
                cResult = CAFF::parseCredits(*block);
                std::cout << "----- CAFF:Credits -----" << std::endl;
                std::cout << cResult->getYear() << std::endl;
                std::cout << cResult->getDay() << std::endl;
                std::cout << cResult->getHour() << std::endl;
                std::cout << cResult->getMonth() << std::endl;
                std::cout << cResult->getCreatorLen() << std::endl;
                std::cout << cResult->getCreator() << std::endl;
                std::cout << cResult->getResult() << std::endl;
            } else if (nextBlockId == 3) {
                CAFF::CaffAnimationResult *aResult = CAFF::parseAnimation(*block);
                std::cout << "----- CAFF:Animation -----" << std::endl;
                std::cout << aResult->getDuration() << std::endl;
                std::cout << aResult->getWidth() << std::endl;
                std::cout << aResult->getHeight() << std::endl;
                std::cout << aResult->getCaption() << std::endl;
                if (aResult->getTags() != nullptr) {
                    for (int i = 0; i < aResult->getTags()->size(); i++) {
                        std::cout << aResult->getTags()->at(i) << std::endl;
                    }
                }

                std::cout << aResult->getResult() << std::endl;

                if (aResult->getResult() == OK_RESULT) {
                    printTGA(std::to_string(animations.size()) + "out.tga", aResult->getWidth(), aResult->getHeight(),
                             aResult->getPixels());
                    animations.push_back(aResult);
                }
            } else {
                std::cout << "Unknown block id!" << std::endl;
            }
            delete block;
            nextBlockId = takeInt(file, 1);
            blockLength = takeInt(file, 8);
        }
    }
    catch (EmptyByteVectorException &e) {
        // TODO write error in json
    }
    catch (MultipleCreditsException &e) {
        // TODO write error in json
    }

    // TODO write preview.tga, metadata.json

    writeJsonCredits(outFile, cResult);
    writeJsonAnimation(outFile, animations);

    delete cResult;

    for (int i = 0; i < animations.size(); i++) {
        delete animations[i]->getTags();
        delete animations[i]->getPixels();
        delete animations[i];
    }

    return 0;
}
