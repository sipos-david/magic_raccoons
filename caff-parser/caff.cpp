#include "caff.hpp"

namespace CAFF
{
    CaffCreditsResult parseCredits(std::vector<byte> &block)
    {
        // TODO check block size is exactly 6 + 8 + creator_len

        const int year = takeInt(block, 2);
        const int month = takeInt(block, 1);
        const int day = takeInt(block, 1);
        const int hour = takeInt(block, 1);
        const int minute = takeInt(block, 1);
        const int creator_len = takeInt(block);

        char creatorArr[creator_len + 1];
        for (size_t i = 0; i < creator_len; i++)
        {
            creatorArr[i] = block.at(i);
        }
        creatorArr[creator_len] = '\0';

        std::string creator(reinterpret_cast<char *>(creatorArr));
        return {year, month, day, hour, minute, creator_len, creator, OK_RESULT};
    }

    CaffAnimationResult parseAnimation(std::vector<byte> &file)
    {
        const int duration = takeInt(file, 8);

        // TODO implement CIFF parsing

        return {duration, OK_RESULT};
    }

    CaffHeaderResult parseCaffHeader(std::vector<byte> &file)
    {
        // Check block id
        const int id = takeInt(file, 1);
        if (id != 1)
        {
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
            caffmagic->at(3) != 0x46)
        {
            delete caffmagic;
            delete rawHeader;
            return {0, 0, "Wrong CAFF Magic!"};
        }

        const int headerSize = takeInt(*rawHeader);
        const int numAnim = takeInt(*rawHeader);

        return {headerSize, numAnim, OK_RESULT};
    }
}