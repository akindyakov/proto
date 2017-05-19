#include "evaluate.h"

#include <tools/tests/ut.h>

#include <iostream>


void singleFunctionsTest() {
    std::cerr << " - singleFunctionsTest\n";
    auto loop = Lisp::Main{};
    {
        auto v = loop.eval(" ( + 2 98 -1) ");
        ValidateEqual(
            v.get<Lisp::Integer>(),
            Lisp::Integer{99}
        );
    }
    {
        auto v = loop.eval(" ( + -2.0 8 1) ");
        ValidateEqual(
            v.get<Lisp::Float>(),
            Lisp::Float{7.0}
        );
    }
    {
        auto v = loop.eval(" ( / -2.0 2 ) ");
        ValidateEqual(
            v.get<Lisp::Float>(),
            Lisp::Float{-1.0}
        );
    }
    {
        auto v = loop.eval(" ( * 2 4 12 -1) ");
        ValidateEqual(
            v.get<Lisp::Integer>(),
            Lisp::Integer{-96}
        );
    }
    {
        auto v = loop.eval(" (abs -11) ");
        ValidateEqual(
            v.get<Lisp::Integer>(),
            Lisp::Integer{11}
        );
    }
}

void nestedFunctionsTest() {
    std::cerr << " - nestedFunctionsTest\n";
    auto loop = Lisp::Main{};
    {
        auto v = loop.eval(" ( + 2 (* 2 3) (/ 3 2 ) ) ");
        ValidateEqual(
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
        ValidateEqual(
            v.get<Lisp::Integer>(),
            Lisp::Integer{2}
        );
    }
}

void defunTest() {
    std::cerr << " - defunTest\n";
    auto loop = Lisp::Main{};
    std::string exp = R"FieldMap(
(defun -dup (n)
  "doc string"
  (* 2 n)
)
(defun +plus (f s)
  "doc string"
  (+ f s)
)
)FieldMap";
    {
        loop.eval(exp);
        auto v = loop.eval("(-dup 3)");
        ValidateEqual(
            v.get<Lisp::Integer>(),
            Lisp::Integer{6}
        );
    }
    {
        loop.eval(exp);
        auto v = loop.eval("(+plus 197 804)");
        ValidateEqual(
            v.get<Lisp::Integer>(),
            Lisp::Integer{1001}
        );
    }
}

void defunRecursiveTest() {
    std::cerr << " - defunRecursiveTest\n";
    auto loop = Lisp::Main{};
    std::string exp = R"FieldMap(
(defun _fib (n)
    "Naive recursive computation of the nth element of the Fibonacci sequence"
    (if (< n 2)
        n
        (+
            (_fib (- n 1))
            (_fib (- n 2))
        )
    )
)
)FieldMap";
    {
        loop.eval(exp);
        auto v = loop.eval("(_fib 4)");
        ValidateEqual(
            v.get<Lisp::Integer>(),
            Lisp::Integer{5}
        );
    }
}

int main() {
    try {
        std::cerr << "evaluate_ut:\n";
        singleFunctionsTest();
        nestedFunctionsTest();
        defunTest();
        defunRecursiveTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
