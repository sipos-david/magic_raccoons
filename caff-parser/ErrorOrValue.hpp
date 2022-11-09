#ifndef CAFF_PARSER_ERROR_OR_VALUE_H
#define CAFF_PARSER_ERROR_OR_VALUE_Hs

template <class Error, class Value>
class ErrorOrValue
{
public:
    ErrorOrValue(Error error) error(error) isError(true) {}
    ErrorOrValue(Value value) value(value) isError(false) {}
    bool isError()
    {
        return isError;
    }
    bool isValue()
    {
        return !isError;
    }
    Value getValue()
    {
        return value;
    }
    Error getError()
    {
        return error;
    }

private:
    Value value;
    Error error;
    bool isError;
};

#endif