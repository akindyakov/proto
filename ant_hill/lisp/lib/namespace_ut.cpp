#include "environment.h"

#include <tools/tests/ut.h>

#include <iostream>

void addNameTest() {
    std::cerr << " - addNameTest\n";
    auto env = Lisp::Env{};
    auto& v = env.addName("first", Lisp::Cell(Lisp::Integer{243}));
    v.get<Lisp::Integer>() += 23;
    ValidateEqual(
        v.get<Lisp::Integer>(),
        Lisp::Integer{266}
    );
    ValidateEqual(
        env.findName("first").get<Lisp::Integer>(),
        Lisp::Integer{266}
    );
    auto popedV = env.popName("first");
    ValidateEqual(
        popedV.get<Lisp::Integer>(),
        Lisp::Integer{266}
    );
    try {
        env.findName("first");
    } catch (const Lisp::Env::Error& err) {
        return;
    }
    throw Exception() << "Expected throwing exception";
}

int main() {
    try {
        std::cerr << "environment_ut:\n";
        addNameTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
