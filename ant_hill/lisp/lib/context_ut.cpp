#include "context.h"

#include <tools/tests/assert.h>

#include <iostream>


void singleFunctionsTest() {
    std::cerr << " - singleFunctionsTest\n";
    auto loop = Lisp::Context{};
    {
        auto v = loop.eval(" ( + 2 98 -1) ");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{99}
        );
    }
    {
        auto v = loop.eval(" ( + -2.0 8 1) ");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Float>(),
            Lisp::Float{7.0}
        );
    }
    {
        auto v = loop.eval(" ( / -2.0 2 ) ");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Float>(),
            Lisp::Float{-1.0}
        );
    }
    {
        auto v = loop.eval(" ( * 2 4 12 -1) ");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{-96}
        );
    }
    {
        auto v = loop.eval(" (abs -11) ");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{11}
        );
    }
}

void nestedFunctionsTest() {
    std::cerr << " - nestedFunctionsTest\n";
    auto loop = Lisp::Context{};
    {
        auto v = loop.eval(" ( + 2 (* 2 3) (/ 3 2 ) ) ");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{9}
        );
    }
    {
        std::string exp = R"FieldMap(
(+
    (*
        (-
            (/ 5 3)
            (rem 7 5)
        )
        (max -2 5 7)
    )
    (abs (min -1 -9 2.0))
)
)FieldMap";
        auto v = loop.eval(exp);
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{2}
        );
    }
}

void defineFuncTest() {
    std::cerr << " - defineFuncTest\n";
    auto loop = Lisp::Context{};
    std::string exp = R"raw(
(define _dup (n)
  "doc string"
  (* 2 n)
)
(define +plus (f s)
  "doc string"
  (+ f s)
)
)raw";
    loop.eval_all(exp);
    {
        auto v = loop.eval("(_dup 3)");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{6}
        );
    }
    {
        auto v = loop.eval("(+plus 197 804)");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{1001}
        );
    }
}

void defineRecursiveFuncTest() {
    std::cerr << " - defineRecursiveFuncTest\n";
    auto loop = Lisp::Context{};
    std::string exp = R"raw(
(define _fib (n)
    "Naive recursive computation of the nth element of the Fibonacci sequence"
    (if (< n 2)
        n
        (+
            (_fib (- n 1))
            (_fib (- n 2))
        )
    )
)
)raw";
    {
        loop.eval(exp);
        auto v = loop.eval("(_fib 2)");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{1}
        );
    }
    {
        loop.eval(exp);
        auto v = loop.eval("(_fib 4)");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{3}
        );
    }
    {
        loop.eval(exp);
        auto v = loop.eval("(_fib 19)");
        UT_ASSERT_EQUAL(
            v.get<Lisp::Integer>(),
            Lisp::Integer{4181}
        );
    }
}

int main() {
    try {
        std::cerr << "context_ut:\n";
        singleFunctionsTest();
        nestedFunctionsTest();
        defineFuncTest();
        defineRecursiveFuncTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
