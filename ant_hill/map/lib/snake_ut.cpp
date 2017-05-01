#include "snake.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void AppearTest() {
//     std::string startText = R"FieldMap(10
// 10
// iiiiiiiiii
// i........i
// i........i
// i........i
// i........i
// i.la...s.i
// i.ws...m.i
// i......i.i
// i........i
// iiiiiiiiii
// )FieldMap";
}

int main(int argn, char** argv) {
    try {
        AppearTest();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
