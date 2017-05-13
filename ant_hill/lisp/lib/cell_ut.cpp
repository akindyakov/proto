#include "cell.h"

#include <tools/tests/ut.h>

#include <iostream>

void NilTest() {
    ValidateEqual(
        Lisp::Nil{} == Lisp::Nil{},
        true
    );
}

template<typename T>
void CheckTagTempl() {
    auto v = Lisp::Cell(T{});
    ValidateEqual(
        static_cast<int>(v.tag()),
        static_cast<int>(Lisp::Cell::tagOf<T>())
    );
}

void CellCheckTypeTag() {
    CheckTagTempl<Lisp::Nil        >();
    CheckTagTempl<Lisp::Cons       >();
    CheckTagTempl<Lisp::Integer    >();
    CheckTagTempl<Lisp::Float      >();
    CheckTagTempl<Lisp::Symbol     >();
    CheckTagTempl<Lisp::String     >();
    CheckTagTempl<Lisp::Table      >();
    CheckTagTempl<Lisp::FunctionPtr>();
}

template<typename T>
void CheckCellGet(const T& bv) {
    auto v = Lisp::Cell(bv);
    ValidateEqual(
        v.get<T>(),
        bv
    );
}

void CellGet() {
    CheckCellGet(Lisp::Integer{123});
    CheckCellGet(Lisp::Float{123.01});
    CheckCellGet(Lisp::Symbol{'%'});
    CheckCellGet(Lisp::String{"What is real?"});
}

int main() {
    try {
        NilTest();
        CellCheckTypeTag();
        CellGet();
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}
