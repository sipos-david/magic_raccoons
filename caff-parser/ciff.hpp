#include "utils.hpp"

namespace CIFF {
    class Header {
        public:
            static Header parseHeader ();
            
            Header (char* magic, int headerSize, int contentSize, int width, int height, char* caption);
            static const int  MAGIC_SIZE       = 4;
            static const int  HEADERSIZE_SIZE  = 8;
            static const int  CONTENTSIZE_SIZE = 8;
            static const int  WIDTH_SIZE       = 8;
            static const int  HEIGHT_SIZE      = 8;
            static const char TAG_SEPARATOR    = '\0';

        private:
            char* magic;
            int headerSize;
            int contentSize;
            int width;
            int height;
            char* caption;

            int actualHeaderSize;
    };

    class Raw{
        public:
            static Raw parseRawCIFF (std::vector<byte> const & buffer);
            Raw ();
            
            Storable toCIFF ();
        private:
            static void saveAsTGA (char* destinationPath);
            
            int actualContentSize;
    };

    class Storable{
        private:
            Storable();
            int len;
    };
}