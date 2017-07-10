#pragma once

#include <lib/validate.h>

namespace Lib {

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

}  // namespace Lib

#define UT_ASSERT_TRUE(value) \
    Lib::validateTrue( \
        value, \
        Lib::UTFailure( \
            __FILE__, \
            __LINE__ \
        ) \
    ); \

#define UT_ASSERT_EQUAL(value, gold) \
    Lib::validateEqual( \
        value, \
        gold, \
        Lib::UTFailure( \
            __FILE__, \
            __LINE__ \
        ) \
    ); \

#define UT_ASSERT_NOT_EQUAL(value, gold) \
    Lib::validateNotEqual( \
        value, \
        gold, \
        Lib::UTFailure( \
            __FILE__, \
            __LINE__ \
        ) \
    ); \
