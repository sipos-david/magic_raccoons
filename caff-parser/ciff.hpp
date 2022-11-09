#ifndef CAFF_PARSER_CIFF_HPP
#define CAFF_PARSER_CIFF_HPP

#include "utils.hpp"
#include "ErrorOrValue.hpp"

namespace CIFF
{
    class Header
    {
    public:
        class InvalidHeaderException : public std::exception
        {
        public:
            char *what()
            {
                return "Custom C++ Exception";
            }
        };
        
        static const Header parseHeader(std::vector<byte>::const_iterator &currentByte);
        static int parseInt(std::vector<byte>::const_iterator &currentByte, const int size);
        static bool parseAndCheckMagic(std::vector<byte>::const_iterator &currentByte);
        static bool isContentSizeValid(const int contentSize, const int width, const int height);
        static ErrorOrValue<InvalidHeaderException, std::string> parseAndCheckCaption(std::vector<byte>::const_iterator &currentByte, const int headerSize);
        static std::vector<std::string> parseTags(std::vector<byte>::const_iterator &currentByte);
        static bool checkTags(const std::vector<std::string> &tags);

        static const std::string MAGIC = "ciff";
        static const int MAGIC_SIZE = 4;
        static const int HEADERSIZE_SIZE = 8;
        static const int CONTENTSIZE_SIZE = 8;
        static const int WIDTH_SIZE = 8;
        static const int HEIGHT_SIZE = 8;
        static const char TAG_SEPARATOR = '\0';

        Header(const int contentSize, const std::string caption, const std::vector<std::string> tags);

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

#endif