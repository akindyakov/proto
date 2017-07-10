#pragma once

#include "cell.h"
#include "namespace.h"

#include <lib/exception.h>

#include <functional>


namespace Lisp {
namespace Func {

class Define
    : public Function
{
public:
    class Error
        : public Lib::Exception
    {
    public:
        explicit Error()
            : Lib::Exception("define: ")
        {
        }
    };

public:
    explicit Define(
        Context& cnt
    )
        : cnt_(cnt)
    {
    }

    Cell call(Function::Args args) const override;

private:
    Context& cnt_;
};

class If
    : public Function
{
public:
    class Error
        : public Lib::Exception
    {
    public:
        explicit Error()
            : Lib::Exception("if: ")
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
        : public Lib::Exception
    {
    public:
        explicit Error()
            : Lib::Exception("eq: ")
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
        : public Lib::Exception
    {
    public:
        explicit Error()
            : Lib::Exception("less: ")
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
        : public Lib::Exception
    {
    public:
        explicit Error()
            : Lib::Exception("greater: ")
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
    class Error
        : public Lib::Exception
    {
    public:
        explicit Error()
            : Lib::Exception("not: ")
        {
        }
    };

public:
    Cell call(Function::Args args) const override;

};

}  // namespace Func
}  // namespace Lisp
