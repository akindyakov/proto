#pragma once

#include "cell.h"


namespace Lisp {

class Env
{
public:
    explicit Env();
    FunctionPtr findFunction(const std::string& name) const;  // ?

    const Cell& findName(const std::string& name) const;
    Cell& findName(const std::string& name);

    Cell& addName(const std::string& name, Cell value);
    Cell popName(const std::string& name);

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
    > funcs;

    std::unordered_map<
        std::string,
        Cell
    > names;
};

}  // namespace Lisp
