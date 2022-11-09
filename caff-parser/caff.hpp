#ifndef CAFF_PARSER_CAFF_HPP
#define CAFF_PARSER_CAFF_HPP

#include "utils.hpp"

namespace CAFF
{
    class ParseResult
    {
    public:
        const std::string getResult() { return result; }
        ParseResult(std::string _result) { result = _result; }

    private:
        std::string result;
    };

    class CaffHeaderResult : public ParseResult
    {
    public:
        const int getHeaderSize() { return headerSize; }
        const int getNumAnim() { return numAnim; }
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
        const int getYear() { return year; }
        const int getMonth() { return month; }
        const int getDay() { return day; }
        const int getHour() { return hour; }
        const int getHeaderSize() { return minute; }
        const int getCreatorLen() { return creatorLen; }
        const std::string getCreator() { return creator; }
        CaffCreditsResult(
            int _year,
            int _month,
            int _day,
            int _hour,
            int _minute,
            int _creatorLen,
            std::string _creator,
            std::string result) : ParseResult(result)
        {
            year = _year;
            month = _month;
            day = _day;
            hour = _hour;
            minute = _minute;
            creator = _creator;
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
        const int getDuration() { return duration; }
        CaffAnimationResult(int _duration, std::string result) : ParseResult(result) { duration = _duration; }

    private:
        int duration;
    };

    CaffCreditsResult* parseCredits(std::vector<byte> &block);

    CaffAnimationResult* parseAnimation(std::vector<byte> &block);

    CaffHeaderResult parseCaffHeader(std::vector<byte> &block);
}

#endif // CAFF_PARSER_CAFF_HPP
