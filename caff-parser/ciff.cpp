#include "ciff.hpp"
using namespace CIFF;

static const Header parseHeader(std::vector<byte>::const_iterator& currentByte)
{
    const std::vector<byte>::const_iterator startingByte = currentByte;
    
    const bool isMagicValid = Header::parseAndCheckMagic (currentByte);

    if (!isMagicValid){
         // to do error handling..
    }
    
    const int headerSize = Header::parseInt(currentByte, Header::HEADERSIZE_SIZE);
    
    const std::vector<byte> contentSizeBytes(currentByte, currentByte + Header::CONTENTSIZE_SIZE);
    const int contentSize = bytesToInt(contentSizeBytes);
    currentByte += Header::CONTENTSIZE_SIZE;

    const std::vector<byte> widthBytes(currentByte, currentByte + Header::WIDTH_SIZE);
    const int width = bytesToInt(widthBytes);
    currentByte += Header::WIDTH_SIZE;

    const std::vector<byte> heightBytes(currentByte, currentByte + Header::HEIGHT_SIZE);
    const int height = bytesToInt(heightBytes);
    currentByte += Header::HEIGHT_SIZE;
    
    const bool isContentSizeValid = parseAndCheckContentSize(currentByte);

    const bool isCaptionValid = Header::parseAndCheckCaption(currentByte);
    const std::vector<std::string> tags = Header::parseTags (currentByte);
    const bool areTagsValid = Header::checkTags (tags);

    const int actualHeaderSize = currentByte - startingByte;
    const bool isHeaderSizeValid = headerSize == actualHeaderSize;

    return new Header(); // to do - mit kell visszaadni
}


static bool parseAndCheckMagic(std::vector<byte>::const_iterator& currentByte){
    bool isMagicValid = true;

    for (int i = 0; i < Header::MAGIC_SIZE; i++){
        currentByte += i;
        isMagicValid &= *currentByte == Header::MAGIC[i];
    }

    return isMagicValid;
}

static int parseInt (std::vector<byte>::const_iterator& currentByte, const int size){
    const std::vector<byte> bytes(currentByte, currentByte + size);
    const int num = bytesToInt(bytes);
    currentByte += size;
    return num;
}


static bool parseAndCheckContentSize (std::vector<byte>::const_iterator& currentByte)
{

}
static bool parseAndCheckCaption (std::vector<byte>::const_iterator& currentByte)
{

}
static std::vector<std::string> parseTags (std::vector<byte>::const_iterator& currentByte)
{

}
static bool checkTags (const std::vector<std::string>& tags)
{
    
}