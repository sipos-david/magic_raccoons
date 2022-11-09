#include "ciff.hpp"
using namespace CIFF;

static const Header parseHeader(std::vector<byte>::const_iterator &currentByte)
{
    const std::vector<byte>::const_iterator startingByte = currentByte;

    const bool isMagicValid = Header::parseAndCheckMagic(currentByte);

    if (!isMagicValid)
    {
        // to do error handling..
    }

    const int headerSize = Header::parseInt(currentByte, Header::HEADERSIZE_SIZE);

    const int contentSize = Header::parseInt(currentByte, Header::CONTENTSIZE_SIZE);
    const int width = Header::parseInt(currentByte, Header::WIDTH_SIZE);
    const int height = Header::parseInt(currentByte, Header::HEIGHT_SIZE);

    const bool isContentSizeValid = Header::isContentSizeValid(contentSize, width, height);

    const ErrorOrValue<Header::InvalidHeaderException, std::string> caption = Header::parseAndCheckCaption(currentByte, headerSize);

    const bool isCaptionValid = Header::parseAndCheckCaption(currentByte, headerSize);
    const std::vector<std::string> tags = Header::parseTags(currentByte);
    const bool areTagsValid = Header::checkTags(tags);

    const int actualHeaderSize = currentByte - startingByte;
    const bool isHeaderSizeValid = headerSize == actualHeaderSize;

    return new Header(); // to do - mit kell visszaadni
}

static bool parseAndCheckMagic(std::vector<byte>::const_iterator &currentByte)
{
    bool isMagicValid = true;

    for (int i = 0; i < Header::MAGIC_SIZE; i++)
    {
        currentByte += i;
        isMagicValid &= *currentByte == Header::MAGIC[i];
    }

    return isMagicValid;
}

static int parseInt(std::vector<byte>::const_iterator &currentByte, const int size)
{
    const std::vector<byte> bytes(currentByte, currentByte + size);
    const int num = bytesToInt(bytes);
    currentByte += size;
    return num;
}

static bool isContentSizeValid(const int contentSize, const int width, const int height)
{
    return contentSize == width * height;
}

static bool parseAndCheckCaption(std::vector<byte>::const_iterator &currentByte, const int headerSize)
{

    while (currentByte)
}

static std::vector<std::string> parseTags(std::vector<byte>::const_iterator &currentByte)
{
}

static bool checkTags(const std::vector<std::string> &tags)
{
}