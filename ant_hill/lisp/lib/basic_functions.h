#pragma once

#include "cell.h"
#include "namespace.h"

#include <functional>


namespace Lisp {
namespace Func {

class Define
    : public Function
{
public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("define: ")
        {
        }
    };

public:
    explicit Define(
        Namespace& nm
    )
        : nm_(nm)
    {
    }

    Cell call(Function::Args args) const override;

private:
    Namespace& nm_;
};

class If
    : public Function
{
public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("if: ")
        {
        }
    };

public:
    Cell call(Function::Args args) const override;

};

class Eq
    : public Function
{
public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("eq: ")
        {
        }
    };

public:
    Cell call(Function::Args args) const override;

};

class Less
    : public Function
{
public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("less: ")
        {
        }
    };

public:
    Cell call(Function::Args args) const override;

};

class Greater
    : public Function
{
public:
    class Error
        : public Exception
    {
    public:
        explicit Error()
            : Exception("greater: ")
        {
        }
    };

public:
    Cell call(Function::Args args) const override;

};

class Not
    : public Function
{
public:
    Cell call(Function::Args args) const override;

};

}  // namespace Func
}  // namespace Lisp
