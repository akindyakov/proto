#include "first.h"
#include "second.h"

#include <iostream>

int main() {
    const auto& f = getFirstText();
    const auto& s = getSecondText();
    std::cerr << (long long)(&f) << " " << (long long)(&s) << std::endl;
    return 0;
}
