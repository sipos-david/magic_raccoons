#include "caff.hpp"

namespace CAFF
{
    CaffCreditsResult parseCredits(std::vector<byte> &file)
    {
        const int year = takeInt(file, 2);
        const int month = takeInt(file, 1);
        const int day = takeInt(file, 1);
        const int hour = takeInt(file, 1);
        const int minute = takeInt(file, 1);
        const int creator_len = takeInt(file);

        std::vector<byte> *rawCreator = take(file, creator_len);
        char creatorArr[creator_len + 1];
        for (size_t i = 0; i < creator_len; i++)
        {
            creatorArr[i] = rawCreator->at(i);
        }
        delete rawCreator;
        creatorArr[creator_len] = '\0';

        std::string creator(reinterpret_cast<char *>(creatorArr));
        return {year, month, day, hour, minute, creator_len, creator, OK_RESULT};
    }

    CaffAnimationResult parseAnimation(std::vector<byte> &file)
    {
        const int duration = takeInt(file, 8);

        return {duration, OK_RESULT};
    }

    CaffHeaderResult parseCaffHeader(std::vector<byte> &file)
    {
        // Check block id
        const int id = takeInt(file, 1);
        if (id != 1)
        {
            std::cout << id;
            return CaffHeaderResult(0, 0, "Wrong CAFF:Header block id!");
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