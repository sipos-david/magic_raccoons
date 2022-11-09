#ifndef UTILS_HPP
#define UTILS_HPP
#include "utils.hpp"
#endif

namespace CIFF
{
    class Header
    {
    public:
        static const Header parseHeader(std::vector<byte>::const_iterator& currentByte);
        static int parseInt (std::vector<byte>::const_iterator& currentByte, const int size);
        static bool parseAndCheckMagic (std::vector<byte>::const_iterator& currentByte);
        static bool parseAndCheckContentSize (std::vector<byte>::const_iterator& currentByte);
        static bool parseAndCheckCaption (std::vector<byte>::const_iterator& currentByte);
        static std::vector<std::string> parseTags (std::vector<byte>::const_iterator& currentByte);
        static bool checkTags (const std::vector<std::string>& tags);

        Header(const int contentSize, const std::string caption, const std::vector<std::string> tags);
        static const std::string MAGIC = "ciff";
        static const int MAGIC_SIZE = 4;
        static const int HEADERSIZE_SIZE = 8;
        static const int CONTENTSIZE_SIZE = 8;
        static const int WIDTH_SIZE = 8;
        static const int HEIGHT_SIZE = 8;
        static const char TAG_SEPARATOR = '\0';

    private:
        char *magic;
        int headerSize;
        int contentSize;
        int width;
        int height;
        char *caption;

        int actualHeaderSize;
    };

    class Storable
    {
    private:
        Storable();
        int len;
    };

    class Raw
    {
    public:
        static Raw parseRawCIFF(std::vector<byte> const &buffer);
        Raw();

        Storable toCIFF();

    private:
        static void saveAsTGA(char *destinationPath);

        int actualContentSize;
    };

};