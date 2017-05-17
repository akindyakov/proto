#pragma once

#include "cell.h"
#include "environment.h"

#include <tools/exception.h>

#include <istream>


namespace Lisp {

class Main
{
public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("Eval error. ")
        {
        }
    };

public:
    explicit Main() = default;

    Main(Main&&) = default;
    Main(const Main&) = delete;

    Main& operator=(Main&&) = default;
    Main& operator=(const Main&) = delete;

    Cell eval(std::istream& in);
    Cell eval(const std::string& expr);

public:
    Env env = Env{};
    std::stack<Env> local;
};

}  // namespace Lisp
