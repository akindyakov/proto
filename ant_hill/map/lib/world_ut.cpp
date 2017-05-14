#include <iostream>

int main() {
    try {
        std::cerr << "world_ut:\n";
        std::cerr << " - TODO\n";
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
