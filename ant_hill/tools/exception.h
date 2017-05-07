#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

class Exception
    : public std::exception
{
private:
    std::string whatHappen;

public:
    Exception() = default;

    Exception(const std::string& what)
        : whatHappen(what) {
    }

    virtual ~Exception() = default;

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

