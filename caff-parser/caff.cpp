#include "caff.hpp"

namespace CAFF {
    CaffCreditsResult *parseCredits(std::vector<byte> &block) {
        const int year = takeInt(block, 2);
        const int month = takeInt(block, 1);
        const int day = takeInt(block, 1);
        const int hour = takeInt(block, 1);
        const int minute = takeInt(block, 1);
        const int creator_len = takeInt(block);

        char creatorArr[creator_len + 1];
        for (size_t i = 0; i < creator_len; i++) {
            creatorArr[i] = block.at(i);
        }
        creatorArr[creator_len] = '\0';

        std::string creator(reinterpret_cast<char *>(creatorArr));
        return new CaffCreditsResult(year, month, day, hour, minute, creator_len, creator, OK_RESULT);
    }

    CaffAnimationResult *parseAnimation(std::vector<byte> &block) {
        const int duration = takeInt(block, 8);

        // check CAFF magic
        const std::vector<byte> *ciffMagic = take(block, 4);
        if ( ciffMagic->at(0) != 0x43 ||
            ciffMagic->at(1) != 0x49 ||
            ciffMagic->at(2) != 0x46 ||
            ciffMagic->at(3) != 0x46) {
            delete ciffMagic;
            return new CaffAnimationResult(duration, 0, 0, "", nullptr, nullptr, "Wrong CIFF magic");
        }
        delete ciffMagic;

        const int header_size = takeInt(block, 8);
        const int content_size = takeInt(block, 8);
        const int width = takeInt(block, 8);
        const int height = takeInt(block, 8);

        const std::vector<byte> *captionByte = takeUntil(block, 0x0A);
        const int captionSize = captionByte->size();
        char captionArr[captionSize];
        for (size_t i = 0; i < captionSize; i++) {
            captionArr[i] = captionByte->at(i);
        }
        captionArr[captionSize - 1] = '\0';
        std::string caption(reinterpret_cast<char *>(captionArr));
        delete captionByte;

        std::vector<byte> *tagsContent = take(block, header_size - (4 + 8 + 8 + 8 + 8 + captionSize));
        std::vector<std::string> *tags = new std::vector<std::string>();

        while (tagsContent->size() != 0) {
            const std::vector<byte> *tagByte = takeUntil(*tagsContent, 0x00);
            const int tagSize = tagByte->size();
            char tagArr[tagSize];
            for (size_t i = 0; i < tagSize; i++) {
                tagArr[i] = tagByte->at(i);
            }
            tagArr[tagSize - 1] = '\0';
            std::string tag(reinterpret_cast<char *>(tagArr));
            tags->push_back(tag);
            delete tagByte;
        }
        delete tagsContent;
        std::vector<Pixel> *pixels = new std::vector<Pixel>();
        int count = 0;
        int red = 0;
        int green = 0;
        int blue = 0;
        for (int i = 0; i < width * height * 3; i++) {
            if (count == 0) {
                red = block.at(i);
            }
            if (count == 1) {
                green = block.at(i);
            }
            if (count == 2) {
                blue = block.at(i);
            }
            count++;
            if (count == 3) {
                pixels->push_back(Pixel(red, green, blue));
                count = 0;
                red = 0;
                green = 0;
                blue = 0;
            }
        }

        return new CaffAnimationResult(duration, width, height, caption, tags, pixels, OK_RESULT);
    }

    CaffHeaderResult parseCaffHeader(std::vector<byte> &file) {
        // Check block id
        const int id = takeInt(file, 1);
        if (id != 1) {
            std::cout << id;
            return {0, 0, "Wrong CAFF:Header block id!"};
        }

        // load CAFF header
        const int headerLength = takeInt(file);
        std::vector<byte> *rawHeader = take(file, headerLength);

        // check CAFF magic
        const std::vector<byte> *caffmagic = take(*rawHeader, 4);
        if (caffmagic->at(0) != 0x43 ||
            caffmagic->at(1) != 0x41 ||
            caffmagic->at(2) != 0x46 ||
            caffmagic->at(3) != 0x46) {
            delete caffmagic;
            delete rawHeader;
            return {0, 0, "Wrong CAFF Magic!"};
        }
        const int headerSize = takeInt(*rawHeader);
        const int numAnim = takeInt(*rawHeader);

        delete caffmagic;
        delete rawHeader;
        return {headerSize, numAnim, OK_RESULT};
    }
}