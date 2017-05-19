#include "basic_functions.h"

#include <tools/tests/ut.h>

#include <iostream>


void ifTest() {
    std::cerr << " - ifTest\n";
    auto if_ = Lisp::Func::If{};
    {
        auto ans = if_.call({
            Lisp::Cell{Lisp::Integer{0}},
            Lisp::Cell{Lisp::Integer{397}},
            Lisp::Cell{Lisp::Integer{123}},
        });
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{397});
    }
    {
        auto ans = if_.call({
            Lisp::Cell{"abc"},
            Lisp::Cell{Lisp::Integer{927}},
            Lisp::Cell{Lisp::Integer{923}},
        });
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::Integer{927});
    }
}

void lessTest() {
    std::cerr << " - lessTest\n";
    auto less = Lisp::Func::Less{};
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{0}},
        });
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::True.get<Lisp::Integer>());
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{1}},
            Lisp::Cell{Lisp::Integer{0}},
        });
        ValidateEqual(ans.is<Lisp::Nil>(), true);
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{1}},
            Lisp::Cell{Lisp::Integer{2}},
            Lisp::Cell{Lisp::Float{3.0}},
            Lisp::Cell{Lisp::Integer{4}},
            Lisp::Cell{Lisp::Integer{5}},
        });
        ValidateEqual(ans.get<Lisp::Integer>(), Lisp::True.get<Lisp::Integer>());
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{1}},
            Lisp::Cell{Lisp::Integer{2}},
            Lisp::Cell{Lisp::Float{1.99}},
            Lisp::Cell{Lisp::Integer{4}},
        });
        ValidateEqual(ans.is<Lisp::Nil>(), true);
    }
}

int main() {
    try {
        std::cerr << "basic_functions_ut:\n";
        ifTest();
        lessTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
