#pragma once

#include "cell.h"
#include "namespace.h"

#include <lib/exception.h>

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
    explicit Context();

    Context(Context&&) = default;
    Context(const Context&) = delete;

    Context& operator=(Context&&) = default;
    Context& operator=(const Context&) = delete;

    Cell eval_all(std::istream& in);
    Cell eval_all(const std::string& expr);
    Cell eval(std::istream& in);
    Cell eval(const std::string& expr);

    Context localContext() {
        return Context(this->ns.createLocal());
    }

    Context isolateContext() {
        return Context(this->ns.createIsolate());
    }

private:

    // Cell setq(std::istream& in);
    // Cell let(std::istream& in);
    // Cell defun(std::istream& in);

    Function::Args readFunctionArguments(std::istream& in);

    explicit Context(
        Namespace ns_
    )
        : ns(std::move(ns_))
    {
    }

public:
    Namespace ns;
};

}  // namespace Lisp
