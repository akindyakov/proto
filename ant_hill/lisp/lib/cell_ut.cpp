#include "cell.h"

#include <tools/tests/ut.h>

#include <iostream>

void nilTest() {
    std::cerr << " - nilTest\n";
    ValidateEqual(
        Lisp::Nil{} == Lisp::Nil{},
        true
    );
}

template<typename T>
void checkTagTempl() {
    auto v = Lisp::Cell(T{});
    ValidateEqual(
        static_cast<int>(v.tag()),
        static_cast<int>(Lisp::Cell::tagOf<T>())
    );
}

void cellCheckTypeTag() {
    std::cerr << " - cellCheckTypeTag\n";
    checkTagTempl<Lisp::Nil        >();
    checkTagTempl<Lisp::Integer    >();
    checkTagTempl<Lisp::Float      >();
    checkTagTempl<Lisp::Symbol     >();
    checkTagTempl<Lisp::String     >();
    checkTagTempl<Lisp::ConsPtr    >();
//    checkTagTempl<Lisp::Table      >();
    checkTagTempl<Lisp::FunctionPtr>();
}

template<typename T>
void checkCellGet(const T& bv) {
    auto v = Lisp::Cell(bv);
    ValidateEqual(
        v.get<T>(),
        bv
    );
}

void cellGet() {
    std::cerr << " - cellGet\n";
    checkCellGet(Lisp::Integer{123});
    checkCellGet(Lisp::Float{123.01});
    checkCellGet(Lisp::Symbol{'%'});
    checkCellGet(Lisp::String{"What is real?"});
}

void cellToStringTest() {
    std::cerr << " - cellToStringTest\n";
    ValidateEqual(
        Lisp::toString(Lisp::Integer{999223}),
        std::string("999223")
    );
    ValidateEqual(
        Lisp::toString(Lisp::Float{342.341000}),
        std::string("342.341000")
    );
    ValidateEqual(
        Lisp::toString(Lisp::Symbol{'@'}),
        std::string("#\\@")
    );
    ValidateEqual(
        Lisp::toString(Lisp::String{"How do you define real?"}),
        std::string("\"How do you define real?\"")
    );
}
int main() {
    try {
        std::cerr << "cell_ut:\n";
        nilTest();
        cellCheckTypeTag();
        cellGet();
        cellToStringTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
