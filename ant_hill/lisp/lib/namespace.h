#pragma once

#include "cell.h"

#include <unordered_map>

namespace Lisp {

class FunctionStorage
{
public:
    FunctionPtr get(const std::string& name) const;

    FunctionPtr add(
        const std::string& name
        , std::unique_ptr<Function> fun
    );

    static FunctionStorage globalStorage();

private:
    std::unordered_map<
        std::string,
        std::unique_ptr<Function>
    > funcs;
};

class Namespace
{
private:
    explicit Namespace(
        FunctionStorage* funcs_
    )
        : funcs(funcs_)
    {
    }

public:
    explicit Namespace();

    FunctionPtr findFunction(const std::string& name) const;  // ?
    Cell addFunction(
        const std::string& name
        , std::unique_ptr<Function> fun
    );

    bool find(const std::string& name, Cell&) const;
    const Cell& find(const std::string& name) const;
    Cell& add(const std::string& name, Cell value);
    void pop(const std::string& name);

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
    std::shared_ptr<FunctionStorage> funcs;
    std::unordered_map<
        std::string,
        Cell
    > names;
};

Namespace createGlobalNamespace();

}  // namespace Lisp
