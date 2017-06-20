#pragma once

#include <tools/validate.h>

class UTFailure
    : public Exception
{
public:
    explicit UTFailure(
        std::string filename
        , int lineNumber
    )
        : Exception()
    {
        (*this) << filename << " +" << lineNumber << ": ";
    }
    virtual ~UTFailure() = default;
};

#define UT_ASSERT_EQUAL(value, gold) \
    Tools::validateEqual( \
        value, \
        gold, \
        UTFailure( \
            __FILE__, \
            __LINE__ \
        ) \
    ); \
