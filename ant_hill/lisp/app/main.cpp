#include <lisp/lib/evaluate.h>

#include <iostream>


int main() {
    auto loop = Lisp::Main{};
    auto counter = int{0};
    while (true) {
        std::cout << "[" << counter++ << "]> ";
        auto ret = loop.eval(std::cin);
        std::cout << ret.toString() << '\n';
    }
    return 0;
}
