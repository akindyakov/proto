#include <lib/map/2d_field.h>

#include <tools/tests/ut.h>

#include <iostream>

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

int main() {
    App();
    return 0;
}
