#include "2d_field.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void PointStreamIO() {
    std::string text = "  ( -123   ,   289 )";
    auto expectedPt = Map::Point{-123, 289};
    auto in = std::istringstream(text);
    auto pt = Map::Point{0, 0};
    in >> pt;
    if (pt != expectedPt) {
        throw AntHill::Exception()
            << "Expected: " << expectedPt.X << " " << expectedPt.Y
            << "\nGot: " << pt.X << " " << pt.Y;
    }
    auto out = std::ostringstream();
    out << pt << expectedPt;
    auto outStr = out.str();
    auto expectedStr = std::string("(-123,289)(-123,289)");
    if (outStr != expectedStr) {
        throw AntHill::Exception()
            << "Expected: " << expectedStr
            << "\nGot: " << outStr;
    }
}

void VectorStreamIO() {
    std::string text = "   <   21239   ,   -2349     >";
    auto expectedPt = Map::Vector{21239, -2349};
    auto in = std::istringstream(text);
    auto vect = Map::Vector{0, 0};
    in >> vect;
    if (vect != expectedPt) {
        throw AntHill::Exception()
            << "Expected: " << expectedPt.X << " " << expectedPt.Y
            << "\nGot: " << vect.X << " " << vect.Y;
    }
    auto out = std::ostringstream();
    out << vect << expectedPt;
    auto outStr = out.str();
    auto expectedStr = std::string("<21239,-2349><21239,-2349>");
    if (outStr != expectedStr) {
        throw AntHill::Exception()
            << "Expected: " << expectedStr
            << "\nGot: " << outStr;
    }
}

void ScanAndPrintField() {
    std::string text = R"FieldMap(<10,10>
(-1,1)
1iiiiiiii2
is......wi
i.s....w.i
i..s..w..i
i...sw...i
i...wa...i
i..w.ms..i
i.w.m..s.i
iw...m..si
4iiiiiiii5
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<char>(in);
    auto element = field.At(Map::Point(4, 6));
    if (element != 'a') {
        throw AntHill::Exception()
            << "Expected: 'a'"
            << "\nGot: '" << element << "'";
    }

    auto out = std::ostringstream();
    Map::PrintToText<char>(out, field);
    auto outText = out.str();
    if (text != outText) {
        throw AntHill::Exception()
            << "Expected: " << text
            << "\nGot: " << outText;
    }
}

int main(int argn, char** argv) {
    try {
        PointStreamIO();
        VectorStreamIO();
        ScanAndPrintField();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
