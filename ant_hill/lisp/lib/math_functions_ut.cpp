#include "math_functions.h"

#include <tools/tests/ut.h>

#include <iostream>


void sumTest() {
    std::cerr << " - sumTest\n";
    {
        auto sum = Lisp::Func::Sum{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Integer{4}),
            Lisp::Cell(Lisp::Integer{6}),
            Lisp::Cell(Lisp::Integer{9}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = sum.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{21});
    }
    {
        auto sum = Lisp::Func::Sum{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Float{2.0}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = sum.call(args);
        ValidateEqual(ans.get<Lisp::Float>(), Lisp::Float{2.0});
    }
}

void productTest() {
    std::cerr << " - productTest\n";
    {
        auto prod = Lisp::Func::Product{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Integer{4}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = prod.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{-8});
    }
    {
        auto prod = Lisp::Func::Product{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Float{1.0}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = prod.call(args);
        ValidateEqual(ans.get<Lisp::Float>(), Lisp::Float{-2.0});
    }
}

void subtractTest() {
    std::cerr << " - subtractTest\n";
    {
        auto sub = Lisp::Func::Subtract{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{934}),
            Lisp::Cell(Lisp::Integer{-34}),
        };
        auto ans = sub.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{968});
    }
    {
        auto sub = Lisp::Func::Subtract{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Float{-123.0}),
            Lisp::Cell(Lisp::Integer{1}),
        };
        auto ans = sub.call(args);
        ValidateEqual(ans.get<Lisp::Float>(), Lisp::Float{-124.0});
    }
}

void divideTest() {
    std::cerr << " - divideTest\n";
    {
        auto div = Lisp::Func::Division{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{128}),
            Lisp::Cell(Lisp::Integer{-64}),
        };
        auto ans = div.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{-2});
    }
    {
        auto div = Lisp::Func::Division{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Float{-12.0}),
            Lisp::Cell(Lisp::Integer{3}),
        };
        auto ans = div.call(args);
        ValidateEqual(ans.get<Lisp::Float>(), Lisp::Float{-4.0});
    }
}

void remainderTest() {
    std::cerr << " - remainderTest\n";
    {
        auto rem = Lisp::Func::Remainder{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{5}),
            Lisp::Cell(Lisp::Integer{2}),
        };
        auto ans = rem.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{1});
    }
    {
        auto rem = Lisp::Func::Remainder{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{7}),
            Lisp::Cell(Lisp::Integer{-3}),
        };
        auto ans = rem.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{3});
    }
}

void absTest() {
    std::cerr << " - absTest\n";
    {
        auto abs = Lisp::Func::Abs{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{-5}),
        };
        auto ans = abs.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{5});
    }
    {
        auto abs = Lisp::Func::Abs{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{123}),
        };
        auto ans = abs.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{123});
    }
    {
        auto abs = Lisp::Func::Abs{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Float{-123.9}),
        };
        auto ans = abs.call(args);
        ValidateEqual(ans.get<Lisp::Float>(), Lisp::Float{123.9});
    }
}

void maxTest() {
    std::cerr << " - maxTest\n";
    {
        auto max = Lisp::Func::Max{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Integer{4}),
            Lisp::Cell(Lisp::Integer{6}),
            Lisp::Cell(Lisp::Integer{9}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = max.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{9});
    }
    {
        auto max = Lisp::Func::Max{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Float{4.0}),
            Lisp::Cell(Lisp::Float{-345.923}),
            Lisp::Cell(Lisp::Integer{389}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = max.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{389});
    }
    {
        auto max = Lisp::Func::Max{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Float{4.0}),
        };
        auto ans = max.call(args);
        ValidateEqual(ans.get<Lisp::Float>(), Lisp::Float{4.0});
    }
}

void minTest() {
    std::cerr << " - minTest\n";
    {
        auto min = Lisp::Func::Min{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Integer{4}),
            Lisp::Cell(Lisp::Integer{-6}),
            Lisp::Cell(Lisp::Integer{9}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = min.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{-6});
    }
    {
        auto min = Lisp::Func::Min{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{1}),
            Lisp::Cell(Lisp::Integer{-2876}),
            Lisp::Cell(Lisp::Float{4.0}),
            Lisp::Cell(Lisp::Float{-345.923}),
            Lisp::Cell(Lisp::Integer{389}),
            Lisp::Cell(Lisp::Integer{-1}),
        };
        auto ans = min.call(args);
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{-2876});
    }
    {
        auto min = Lisp::Func::Min{};
        auto args = Lisp::Function::Args{
            Lisp::Cell(Lisp::Integer{3}),
            Lisp::Cell(Lisp::Integer{2}),
            Lisp::Cell(Lisp::Float{1.0}),
        };
        auto ans = min.call(args);
        ValidateEqual(ans.get<Lisp::Float>(), Lisp::Float{1.0});
    }
}

int main() {
    try {
        std::cerr << "math_functions_ut:\n";
        sumTest();
        productTest();
        subtractTest();
        divideTest();
        absTest();
        maxTest();
        minTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
