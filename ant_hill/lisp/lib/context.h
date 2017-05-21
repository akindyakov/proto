#pragma once

#include "cell.h"
#include "namespace.h"

#include <tools/exception.h>

#include <istream>
#include <memory>


namespace Lisp {

class Context
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
    explicit Context() = default;

    Context(Context&&) = default;
    Context(const Context&) = delete;

    Context& operator=(Context&&) = default;
    Context& operator=(const Context&) = delete;

    Cell eval_all(std::istream& in);
    Cell eval_all(const std::string& expr);
    Cell eval(std::istream& in);
    Cell eval(const std::string& expr);

    Cell findName(const std::string& name) const;
    Cell addName(const std::string& name, Cell value);

    Context localContext() const;

private:

    Cell setq(std::istream& in);
    Cell let(std::istream& in);
    Cell defun(std::istream& in);

    Function::Args readFunctionArguments(std::istream& in);

public:
    Namespace nm;
    std::unique_ptr<Namespace> externalNm;
};

}  // namespace Lisp
