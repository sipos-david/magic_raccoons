#include "caff.hpp"

namespace CAFF
{
    int parseCredits(std::vector<byte> &file) {}

    int parseAnimation(std::vector<byte> &file) {}

    CaffHeaderResult parseCaffHeader(std::vector<byte> &file)
    {
        // Check block id
        const int id = takeInt(file, 1);
        if (id != 1)
        {
            return CaffHeaderResult(0, 0, "Wrong block id!");
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