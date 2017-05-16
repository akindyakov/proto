#pragma once

#include "cell.h"


namespace Lisp {

class Env
{
public:
    explicit Env();
    const Function& findFunction(const std::string& name) const;  // ?

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
        FunctionPtr
    > global;
};

}  // namespace Lisp
