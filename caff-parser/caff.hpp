#ifndef UTILS_HPP
#define UTILS_HPP
#include "utils.hpp"
#endif

namespace CAFF
{
    class ParseResult
    {
    public:
        const std::string const getResult() { return result; }
        ParseResult(std::string _result) { result = _result; }

    private:
        std::string result;
    };

    class CaffHeaderResult : public ParseResult
    {
    public:
        const int const getHeaderSize() { return headerSize; }
        const int const getNumAnim() { return numAnim; }
        CaffHeaderResult(int _headerSize, int _numAnim, std::string result) : ParseResult(result)
        {
            headerSize = _headerSize;
            numAnim = _numAnim;
        }

    private:
        int headerSize;
        int numAnim;
    };

    class CaffCreditsResult : public ParseResult
    {
    public:
        const int getYear() const { return year; }
        const int getMonth() const { return month; }
        const int getDay() const { return day; }
        const int getHour() const { return hour; }
        const int getHeaderSize() const { return minute; }
        const int getCreatorLen() const { return creatorLen; }
        const std::string const getCreator() { return creator; }
        CaffCreditsResult(
            int _year,
            int _month,
            int _day,
            int _hour,
            int _minute,
            int _creatorLen,
            std::string result) : ParseResult(result)
        {
            year = _year;
            month = _month;
            day = _day;
            hour = _hour;
            minute = _minute;
            creatorLen = _creatorLen;
        }

    private:
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int creatorLen;
        std::string creator;
    };

    class CaffAnimationResult : public ParseResult
    {
    public:
        int duration;
    };

    int parseCredits(std::vector<byte> &file);

    int parseAnimation(std::vector<byte> &file);

    CaffHeaderResult parseCaffHeader(std::vector<byte> &file);
}