#pragma once

#include "cell.h"
#include "namespace.h"

#include <tools/exception.h>

#include <istream>


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

    Cell eval(std::istream& in);
    Cell eval(const std::string& expr);

    void pushStackFrame(LocalEnv);
    void popStackFrame();

    Cell findName(const std::string& name) const;

private:
    Cell eval_one(std::istream& in);

    Cell setq(std::istream& in);
    Cell let(std::istream& in);
    Cell defun(std::istream& in);

    Function::Args readFunctionArguments(std::istream& in);

    static void readParenthesesBegin(std::istream& in);
    static void readParenthesesEnd(std::istream& in);

public:
    Namespace env = Namespace{};
    std::shared_ptr<Namespace> externalEnv;
    //std::vector<LocalEnv> localEnv;
    //std::vector<LocalEnv> localEnv;

};

}  // namespace Lisp
