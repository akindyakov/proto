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

private:
    std::unordered_map<
        std::string,
        std::unique_ptr<Function>
    > funcs;
};

class Namespace
{
public:
    static Namespace createGlobal();
    Namespace createLocal();
    Namespace createIsolate();

    const Cell& add(const std::string& name, Cell value);
    const Cell& addGlobal(const std::string& name, Cell value);
    void pop(const std::string& name);

    bool find(const std::string& name, Cell&) const;
    Cell find(const std::string& name) const;

    FunctionPtr findFunction(const std::string& name) const;
    Cell addFunction(
        const std::string& name
        , std::unique_ptr<Function> fun
    );

private:
    explicit Namespace(
        Namespace* ext_
        , std::unique_ptr<FunctionStorage> funcs_ = nullptr
    )
        : ext(ext_)
        , funcs(std::move(funcs_))
    {
    }
    Namespace* findGlobal();
    const Namespace* findGlobal() const;

public:
    class Error
        : public Lib::Exception
    {
    public:
        explicit Error()
            : Lib::Exception("Evironment error. ")
        {
        }
    };

private:
    Namespace* ext;
    std::unique_ptr<FunctionStorage> funcs;
    std::unordered_map<
        std::string,
        Cell
    > names;
};

}  // namespace Lisp
