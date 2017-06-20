#pragma once

#include <tools/exception.h>


template<typename T>
inline void Validate(
    const T& value
    , const T& expected
) {
    if (value != expected) {
        throw Exception()
            << "Wrong value: '" << value << "'."
            << " Expected: '" << expected << "'.";
        ;
    }
}

template<
    typename FirstT
    , typename SecondT
>
inline void ValidateEqual(
    const FirstT& value
    , const SecondT& other
) {
    if (!(value == other)) {
        throw Exception()
            << "'" << value << "' is suppose to be equal to '" << other << "'."
        ;
    }
}

template<
    typename FirstT
    , typename SecondT
>
inline void ValidateNotEqual(
    const FirstT& value
    , const SecondT& other
) {
    if (!(value != other)) {
        throw Exception()
            << "'" << value << "' is suppose to be not equal to '" << other << "'."
        ;
    }
}
