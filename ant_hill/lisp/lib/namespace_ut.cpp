#include "namespace.h"

#include <lib/tests/assert.h>

#include <iostream>

void addNameTest() {
    std::cerr << " - addNameTest\n";
    auto env = Lisp::Namespace::createGlobal();
    auto& v = env.add("first", Lisp::Cell(Lisp::Integer{243}));
    UT_ASSERT_EQUAL(
        v.get<Lisp::Integer>(),
        Lisp::Integer{243}
    );
    auto& vv = env.add("first", Lisp::Cell(Lisp::Integer{9126}));
    UT_ASSERT_EQUAL(
        vv.get<Lisp::Integer>(),
        Lisp::Integer{9126}
    );
    UT_ASSERT_EQUAL(
        env.find("first").get<Lisp::Integer>(),
        Lisp::Integer{9126}
    );
    env.pop("first");
    try {
        env.find("first");
    } catch (const Lisp::Namespace::Error& err) {
        return;
    }
    throw Lib::Exception() << "Expected throwing exception";
}

int main() {
    try {
        std::cerr << "namespace_ut:\n";
        addNameTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
