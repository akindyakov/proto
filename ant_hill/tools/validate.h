#pragma once

#include <tools/str/to_string.h>

#include <tools/exception.h>


namespace Tools {

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
            << "[" << Tools::toString(value)
            << "] is suppose to be equal to ["
            << Tools::toString(other)
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
            << "[" << Tools::toString(value)
            << "] is suppose to be not equal to ["
            << Tools::toString(other)
            << "]."
        ;
    }
}

}
