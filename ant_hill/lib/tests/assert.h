#pragma once

#include <lib/validate.h>

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

#define UT_ASSERT(value) \
    Lib::validateTrue( \
        value, \
        UTFailure( \
            __FILE__, \
            __LINE__ \
        ) \
    ); \

#define UT_ASSERT_EQUAL(value, gold) \
    Lib::validateEqual( \
        value, \
        gold, \
        UTFailure( \
            __FILE__, \
            __LINE__ \
        ) \
    ); \

#define UT_ASSERT_NOT_EQUAL(value, gold) \
    Lib::validateNotEqual( \
        value, \
        gold, \
        UTFailure( \
            __FILE__, \
            __LINE__ \
        ) \
    ); \
