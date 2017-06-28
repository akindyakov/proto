#pragma once

#include <lib/str/to_string.h>

#include <lib/exception.h>


namespace Lib {

template<
    typename FirstT
    , typename ExceptionType = Exception
>
inline void validateTrue(
    const FirstT& value
    , ExceptionType error = ExceptionType{}
) {
    if (!value) {
        throw error << "[" << Lib::toString(value) << "] is suppose to be true";
    }
}

template<
    typename FirstT
    , typename SecondT
    , typename ExceptionType = Exception
>
inline void validateEqual(
    const FirstT& value
    , const SecondT& other
    , ExceptionType error = ExceptionType{}
) {
    if (!(value == other)) {
        throw error
            << "[" << Lib::toString(value)
            << "] is suppose to be equal to ["
            << Lib::toString(other)
            << "]."
        ;
    }
}

template<
    typename FirstT
    , typename SecondT
    , typename ExceptionType = Exception
>
inline void validateNotEqual(
    const FirstT& value
    , const SecondT& other
    , ExceptionType error = ExceptionType{}
) {
    if (!(value != other)) {
        throw error
            << "[" << Lib::toString(value)
            << "] is suppose to be not equal to ["
            << Lib::toString(other)
            << "]."
        ;
    }
}

}  // namespace Lib
