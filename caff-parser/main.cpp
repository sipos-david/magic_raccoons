#include "caff.hpp"
#include <regex>

std::vector<byte> loadFile(std::string const &filepath) {
    try {
        std::ifstream input(filepath, std::ios::binary);

        // copies all data into buffer
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
        return buffer;
    } catch (...) {
        return std::vector<unsigned char>();
    }
}

void writeJsonError(std::ofstream &outFile, const std::string &error) {
    std::cout << error << std::endl;
    outFile << "{" << std::endl;
    outFile << "\t\"error\": \"" << error << "\"" << std::endl;
    outFile << "}" << std::endl;
    outFile.close();
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
        outFile << " " << aResult[i]->getDuration() << "," << std::endl;
        outFile << "\t\t\"width\":" << aResult[i]->getWidth();
        outFile << "," << std::endl;
        outFile << "\t\t\"height\":" << aResult[i]->getHeight();
        outFile << "," << std::endl;
        outFile << "\t\t\"caption\":\"" << aResult[i]->getCaption();
        outFile << "\"," << std::endl;
        outFile << "\t\t\"tags\":[";
        for (size_t j = 0; j < aResult[i]->getTags()->size(); j++) {
            outFile << "\"" << aResult[i]->getTags()->at(j) << "\"";
            if (j != aResult[i]->getTags()->size() - 1) {
                outFile << "," << std::endl;
            }
        }
        outFile << "\t\t]" << std::endl;
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

void clean(CAFF::CaffCreditsResult *cResult, std::vector<CAFF::CaffAnimationResult *> animations) {
    delete cResult;

    for (int i = 0; i < animations.size(); i++) {
        delete animations[i]->getTags();
        delete animations[i]->getPixels();
        delete animations[i];
    }
}

int main(int argc, char const *argv[]) {
    // argv[1] => input file
    // argv[2] => output folder without / at the end (preview.tga, metadata.json)

    if (argc != 3) {
        // args not provided
        return 1;
    }

    std::string outputFolder = argv[2];

    const std::string tgaBasePath = outputFolder + "/preview";
    const std::string tgaExtension = ".tga";
    const std::string jsonPath = outputFolder + "/metadata.json";

    std::ofstream metadata(jsonPath);
    if (!metadata) {
        std::cout << jsonPath << std::endl;
        return 1;
    }

    std::vector<byte> file = loadFile(argv[1]);
    if (file.empty()) {
        writeJsonError(metadata, "Input file error!");
        return 1;
    }

    CAFF::CaffHeaderResult hResult = CAFF::parseCaffHeader(file);
    CAFF::CaffCreditsResult *cResult = nullptr;
    std::vector<CAFF::CaffAnimationResult *> animations;

    if (hResult.getResult() != OK_RESULT) {
        writeJsonError(metadata, hResult.getResult());
        return 1;
    }

    try {
        while (!file.empty()) {
            int nextBlockId = takeInt(file, 1);
            int blockLength = takeInt(file, 8);

            std::vector<byte> *block = take(file, blockLength);
            if (nextBlockId == 2) {
                if (cResult != nullptr) {
                    throw MultipleCreditsException();
                }
                cResult = CAFF::parseCredits(*block);
                if (cResult->getResult() != OK_RESULT) {
                    writeJsonError(metadata, cResult->getResult());
                    clean(cResult, animations);
                    return 1;
                }
            } else if (nextBlockId == 3) {
                CAFF::CaffAnimationResult *aResult = CAFF::parseAnimation(*block);
                animations.push_back(aResult);
                if (aResult->getResult() == OK_RESULT) {
                    printTGA(tgaBasePath + std::to_string(animations.size()) + tgaExtension,
                             aResult->getWidth(),
                             aResult->getHeight(),
                             aResult->getPixels());
                } else {
                    writeJsonError(metadata, aResult->getResult());
                    clean(cResult, animations);
                    return 1;
                }
            } else {
                writeJsonError(metadata, "Unknown block id in file!");
                clean(cResult, animations);
                return 1;
            }
            delete block;
        }
    } catch (std::exception &e) {
        writeJsonError(metadata, e.what());
        clean(cResult, animations);
        return 1;
    }

    writeJsonCredits(metadata, cResult);
    writeJsonAnimation(metadata, animations);

    clean(cResult, animations);

    return 0;
}
