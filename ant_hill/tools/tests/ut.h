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

template<typename T>
inline void ValidateEqual(
    const T& value
    , const T& other
) {
    if (!(value == other)) {
        throw Exception()
            << "'" << value << " is suppose to be equal to '" << other << "'."
        ;
    }
}

template<typename T>
inline void ValidateNotEqual(
    const T& value
    , const T& other
) {
    if (!(value != other)) {
        throw Exception()
            << "'" << value << " is suppose to be not equal to '" << other << "'."
        ;
    }
}

