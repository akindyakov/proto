#include "has_method.h"

#include <iostream>

class TReturn {
public:
    TReturn() = default;
};

class TArg {
public:
    TArg() = delete;
    explicit TArg(int, float);
};

CREATE_HAS_METHOD(TReturn, First, int{}, TArg{int{}, float{}});

class TTrueTest {
public:
    TReturn First(int, TArg);
};

class TWrongAll {
};

class TWrongReturnValue {
    int First(int, TArg);
};

class TWrongArgs1Value {
    TReturn First(int, TArg, double);
};

class TWrongArgs2Value {
    TReturn First(int);
};

class TWrongArgs3Value {
    TReturn First();
};

int main() {
    static_assert(has_First<TTrueTest>::value == true, "TTrueTest failed");
    static_assert(has_First<TWrongAll>::value == false, "TWrongAll failed");
    static_assert(has_First<TWrongReturnValue>::value == false, "TWrongReturnValueled failed");
    static_assert(has_First<TWrongArgs1Value>::value == false,  "TWrongArgs1Value failed");
    static_assert(has_First<TWrongArgs2Value>::value == false,  "TWrongArgs2Value failed");
    static_assert(has_First<TWrongArgs3Value>::value == false,  "TWrongArgs3Value failed");
    return 0;
}
