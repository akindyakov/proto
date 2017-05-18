#pragma once

#include "cell.h"

#include <functional>


namespace Lisp {
namespace Func {

template<
    typename Operation
    , typename TError
>
class Commutative
    : public Function
{
public:
    Operation op;

public:
    Cell call(Function::Args args) const override {
        if (args.empty()) {
            throw TError() << "given too few arguments";
        }
        auto it = args.cbegin();
        Cell value = *it;
        ++it;
        while (it != args.cend()) {
            value = op(value, *it);
            ++it;
        }
        return value;
    }
};

template<
    typename Operation
>
class RealNumberOperation
{
public:
    Operation op;

public:
    Cell operator() (
        const Cell& first
        , const Cell& second
    ) const {
        if (first.is<Integer>() && second.is<Integer>()) {
            return Cell(
                op(
                    first.get<Integer>(),
                    second.get<Integer>()
                )
            );
        } else if (first.is<Float>() && second.is<Float>()) {
            return Cell(op(
                first.get<Float>(),
                second.get<Float>()
            ));
        } else if (first.is<Float>() && second.is<Integer>()) {
            return Cell(op(
                first.get<Float>(),
                second.get<Integer>()
            ));
        } else if (first.is<Integer>() && second.is<Float>()) {
            return Cell(op(
                static_cast<Float>(first.get<Integer>()),
                second.get<Float>()
            ));
        }
        // throw an Exception
        first.asRealNumber();
        second.asRealNumber();
        return first;
    }
};

/**
* (+ x1 x2 ... xn)    The sum of x1, x2, ..., xn
*/
class SumError
    : public Exception
{
public:
    explicit SumError()
        : Exception("SUM: ")
    {
    }
};
using Sum = Commutative<RealNumberOperation<std::plus<>>, SumError>;

// /**
// * (* x1 x2 ... xn)    The product of x1, x2, ..., xn
// */
class ProductError
    : public Exception
{
public:
    explicit ProductError()
        : Exception("PRODUCT: ")
    {
    }
};
using Product = Commutative<
    RealNumberOperation<std::multiplies<>>,
    ProductError
>;

template<
    typename Operation
    , typename TError
>
class Anticommutative
    : public Function
{
public:
    Operation op;

public:
    Cell call(Function::Args args) const override {
        if (args.size() < 2) {
            throw TError() << "given too few arguments: " << args.size();
        }
        if (args.size() > 2) {
            throw TError() << "given too many arguments: " << args.size();
        }
        return op(args.front(), args.back());
    }
};

/**
* (- x y) Subtract y from x
*/
class SubtractError
    : public Exception
{
public:
    explicit SubtractError()
        : Exception("SUBTRACT: ")
    {
    }
};
using Subtract = Anticommutative<
    RealNumberOperation<std::minus<>>,
    SubtractError
>;

/**
* (/ x y) Divide x by y
*/
class DivisionError
    : public Exception
{
public:
    explicit DivisionError()
        : Exception("DIVISION: ")
    {
    }
};
using Division = Anticommutative<
    RealNumberOperation<std::divides<>>,
    DivisionError
>;

template<
    typename Operation
>
class IntegerOperation
{
public:
    Operation op;

public:
    Cell operator() (
        const Cell& first
        , const Cell& second
    ) const {
        return Cell(
            op(
                first.get<Integer>(),
                second.get<Integer>()
            )
        );
    }
};

/**
* (rem x y)   The remainder of dividing x by y
*/
class RemainderError
    : public Exception
{
public:
    explicit RemainderError()
        : Exception("REMAINDER: ")
    {
    }
};
using Remainder = Anticommutative<
    IntegerOperation<std::modulus<>>,
    RemainderError
>;

/**
* (abs x) The absolute value of x
*/
class AbsError
    : public Exception
{
public:
    explicit AbsError()
        : Exception("ABS: ")
    {
    }
};

class Abs
    : public Function
{
public:
    Cell call(Function::Args args) const override;
};

/**
* (max x1 x2 ... xn)  The maximum of x1, x2, ..., xn
*/
class MaxError
    : public Exception
{
public:
    explicit MaxError()
        : Exception("MAX: ")
    {
    }
};
class Max
    : public Function
{
public:
    Cell call(Function::Args args) const override;

};

/**
* (min x1 x2 ... xn)  The minimum of x1, x2, ..., xn
*/
class MinError
    : public Exception
{
public:
    explicit MinError()
        : Exception("MIN: ")
    {
    }
};
class Min
    : public Function
{
public:
    Cell call(Function::Args args) const override;

};

}  // namespace Func
}  // namespace Lisp
