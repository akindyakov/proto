#pragma once

#include "cell.h"


namespace Lisp {

class Env
{
public:
    explicit Env();
    FunctionPtr findFunction(const std::string& name) const;  // ?

public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("Evironment error. ")
        {
        }
    };

private:
    std::unordered_map<
        std::string,
        std::unique_ptr<Function>
    > global;
};

}  // namespace Lisp
