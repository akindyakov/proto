#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

//namespace AntHill {

class Exception: public std::exception {
private:
    std::string whatHappen;

public:
    Exception() {}
    Exception(const std::string& what)
        : whatHappen(what) {
    }

    template<typename T>
    Exception& operator<<(const T& val) {
        std::ostringstream ostr(whatHappen, std::ios_base::ate);
        ostr << val;
        whatHappen = ostr.str();
        return *this;
    }

    const char* what() const throw() override {
        return whatHappen.c_str();
    }
};

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

//}
