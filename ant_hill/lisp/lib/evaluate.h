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

    void pushStackFrame(LocalEnv);
    void popStackFrame();

    Cell findName(const std::string& name) const;

private:
    Cell eval_one(std::istream& in);

    Cell setq(std::istream& in);
    Cell let(std::istream& in);
    Cell defun(std::istream& in);

    Function::Args readFunctionArguments(std::istream& in);

public:
    Env globalEnv = Env{};
    std::vector<LocalEnv> localEnv;
};

}  // namespace Lisp
