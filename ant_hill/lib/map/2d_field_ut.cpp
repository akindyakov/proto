#include "2d_field.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void App() {
    std::string text = R"FieldMap(10
10
iiiiiiiiii
is......wi
i.s....w.i
i..s..w..i
i...sw...i
i...ws...i
i..w.ms..i
i.w.m..s.i
iw...m..si
iiiiiiiiii
)FieldMap";
    auto in = std::istringstream(text);
    auto field = NField::ScanFromText(in);
    auto out = std::ostringstream();
    NField::PrintToText(out, field);
    auto outText = out.str();
    if (text != outText) {
        throw NAntHill::TException()
            << "Expected: " << text
            << "\nGot: " << outText;
    }
}

int main(int argn, char** argv) {
    try {
        App();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
