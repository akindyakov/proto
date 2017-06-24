#include "basic_functions.h"

#include <lib/tests/assert.h>

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
        UT_ASSERT_EQUAL(ans.get<Lisp::Integer>(), Lisp::Integer{397});
    }
    {
        auto ans = if_.call({
            Lisp::Cell{"abc"},
            Lisp::Cell{Lisp::Integer{927}},
            Lisp::Cell{Lisp::Integer{923}},
        });
        UT_ASSERT_EQUAL(ans.get<Lisp::Integer>(), Lisp::Integer{927});
    }
}

void lessTest() {
    std::cerr << " - lessTest\n";
    auto less = Lisp::Func::Less{};
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{0}},
        });
        UT_ASSERT_EQUAL(ans.get<Lisp::Integer>(), Lisp::True.get<Lisp::Integer>());
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{1}},
            Lisp::Cell{Lisp::Integer{0}},
        });
        UT_ASSERT_EQUAL(ans.is<Lisp::Nil>(), true);
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{1}},
            Lisp::Cell{Lisp::Integer{2}},
            Lisp::Cell{Lisp::Float{3.0}},
            Lisp::Cell{Lisp::Integer{4}},
            Lisp::Cell{Lisp::Integer{5}},
        });
        UT_ASSERT_EQUAL(ans.get<Lisp::Integer>(), Lisp::True.get<Lisp::Integer>());
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{1}},
            Lisp::Cell{Lisp::Integer{2}},
            Lisp::Cell{Lisp::Float{1.99}},
            Lisp::Cell{Lisp::Integer{4}},
        });
        UT_ASSERT_EQUAL(ans.is<Lisp::Nil>(), true);
    }
}

void notTest() {
    std::cerr << " - notTest\n";
    const auto less = Lisp::Func::Not{};
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Nil{}},
        });
        UT_ASSERT_EQUAL(ans.get<Lisp::Integer>(), Lisp::True.get<Lisp::Integer>());
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::True},
        });
        UT_ASSERT_EQUAL(ans.is<Lisp::Nil>(), true);
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Integer{89}},
        });
        UT_ASSERT_EQUAL(ans.is<Lisp::Nil>(), true);
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::Float{13.412}},
        });
        UT_ASSERT_EQUAL(ans.is<Lisp::Nil>(), true);
    }
    {
        auto ans = less.call({
            Lisp::Cell{Lisp::String{"rose rain"}},
        });
        UT_ASSERT_EQUAL(ans.is<Lisp::Nil>(), true);
    }
}

int main() {
    try {
        std::cerr << "basic_functions_ut:\n";
        ifTest();
        lessTest();
        notTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
