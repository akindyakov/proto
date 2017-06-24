#include "2d_field.h"

#include <lib/tests/assert.h>

#include <iostream>
#include <sstream>
#include <string>

void compareTest() {
    std::cerr << " - compareTest\n";
    UT_ASSERT_EQUAL(
        Map::Point(-12, 317),
        Map::Point(-12, 317)
    );
    UT_ASSERT_NOT_EQUAL(
        Map::Point(12, 317),
        Map::Point(-12, 317)
    );
    UT_ASSERT_NOT_EQUAL(
        Map::Point(12, 17),
        Map::Point(-12, 317)
    );
    UT_ASSERT_NOT_EQUAL(
        Map::Point(12, 17),
        Map::Point(12, 7)
    );

    UT_ASSERT_EQUAL(
        Map::Vector(-12, 317),
        Map::Vector(-12, 317)
    );
    UT_ASSERT_NOT_EQUAL(
        Map::Vector(12, 317),
        Map::Vector(-12, 317)
    );
    UT_ASSERT_NOT_EQUAL(
        Map::Vector(12, 17),
        Map::Vector(-12, 317)
    );
    UT_ASSERT_NOT_EQUAL(
        Map::Vector(12, 17),
        Map::Vector(12, 7)
    );
}

void pointStreamIO() {
    std::cerr << " - pointStreamIO\n";
    std::string text = "  ( -123   ,   289 )";
    auto expectedPt = Map::Point{-123, 289};
    auto in = std::istringstream(text);
    auto pt = Map::Point{0, 0};
    in >> pt;
    if (pt != expectedPt) {
        throw Exception()
            << "Expected: " << expectedPt.X << " " << expectedPt.Y
            << "\nGot: " << pt.X << " " << pt.Y;
    }
    auto out = std::ostringstream();
    out << pt << expectedPt;
    auto outStr = out.str();
    auto expectedStr = std::string("(-123,289)(-123,289)");
    if (outStr != expectedStr) {
        throw Exception()
            << "Expected: " << expectedStr
            << "\nGot: " << outStr;
    }
}

void vectorStreamIO() {
    std::cerr << " - vectorStreamIO\n";
    std::string text = "   <   21239   ,   -2349     >";
    auto expectedPt = Map::Vector{21239, -2349};
    auto in = std::istringstream(text);
    auto vect = Map::Vector{0, 0};
    in >> vect;
    if (vect != expectedPt) {
        throw Exception()
            << "Expected: " << expectedPt.X << " " << expectedPt.Y
            << "\nGot: " << vect.X << " " << vect.Y;
    }
    auto out = std::ostringstream();
    out << vect << expectedPt;
    auto outStr = out.str();
    auto expectedStr = std::string("<21239,-2349><21239,-2349>");
    if (outStr != expectedStr) {
        throw Exception()
            << "Expected: " << expectedStr
            << "\nGot: " << outStr;
    }
}

void inRangeFieldTest() {
    std::cerr << " - inRangeFieldTest\n";
    std::string text = R"FieldMap(<8,8>
(0,0)
........
........
........
........
........
........
........
........
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    UT_ASSERT_EQUAL(field.inRange(Map::Point( 0, 0)), true);
    UT_ASSERT_EQUAL(field.inRange(Map::Point( 0, 1)), true);
    UT_ASSERT_EQUAL(field.inRange(Map::Point( 0, 3)), true);
    UT_ASSERT_EQUAL(field.inRange(Map::Point( 1, 0)), true);
    UT_ASSERT_EQUAL(field.inRange(Map::Point( 1, 1)), true);

    UT_ASSERT_EQUAL(field.inRange(Map::Point( 8, 5)), false);
}

void scanAndPrintField() {
    std::cerr << " - scanAndPrintField\n";
    std::string text = R"FieldMap(<10,10>
(-1,1)
.iiiiiiii.
is......wi
i.s....w.i
i..s..w..i
i...sw...i
i...wa...i
i..w.ms..i
i.w.m..s.i
iw...m..si
.iiiiiiii.
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto element = field.at(Map::Point(4, 6));
    UT_ASSERT_EQUAL(element.grain, Map::EMaterial::Water);

    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    auto outText = out.str();

    UT_ASSERT_EQUAL(text, outText);
}

int main(int argn, char** argv) {
    try {
        std::cerr << "2d_field_ut:\n";
        pointStreamIO();
        vectorStreamIO();
        inRangeFieldTest();
        scanAndPrintField();
        compareTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
