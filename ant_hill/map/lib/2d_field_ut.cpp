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

void vectorAbsTest() {
    std::cerr << " - vectorAbsTest\n";
    UT_ASSERT_EQUAL(
        Map::Vector(28, 21).abs(),
        Map::Vector(28, 21)
    );
    UT_ASSERT_EQUAL(
        Map::Vector(-35, 81).abs(),
        Map::Vector( 35, 81)
    );
    UT_ASSERT_EQUAL(
        Map::Vector(-97, -22).abs(),
        Map::Vector( 97,  22)
    );
}

void vectorInverseTest() {
    std::cerr << " - vectorInverseTest\n";
    UT_ASSERT_EQUAL(
        Map::Vector( 28,  21).inverse(),
        Map::Vector(-28, -21)
    );
    UT_ASSERT_EQUAL(
        Map::Vector(-35,  81).inverse(),
        Map::Vector( 35, -81)
    );
    UT_ASSERT_EQUAL(
        Map::Vector(-97, -22).inverse(),
        Map::Vector( 97,  22)
    );
    static_assert(
        Map::Vector(-97, -22).inverse() == Map::Vector( 97,  22),
        "is suppose to be equal"
    );
}

void vectorNormalizeTest() {
    std::cerr << " - vectorNormalizeTest\n";
    UT_ASSERT_EQUAL(
        Map::Vector(28, 21).normalize(),
        Map::Vector( 1,  1)
    );
    UT_ASSERT_EQUAL(
        Map::Vector(-35, 81).normalize(),
        Map::Vector( -1,  1)
    );
    UT_ASSERT_EQUAL(
        Map::Vector(0, -22).normalize(),
        Map::Vector(0,  -1)
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
    UT_ASSERT_EQUAL(element.grain, Map::EMaterial::Letter_a);

    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    auto outText = out.str();

    UT_ASSERT_EQUAL(text, outText);
}

void squareIteratorTest() {
    std::cerr << " - squareIteratorTest\n";
    auto iterator = Map::SquareIterator(
        Map::Square(
            Map::Vector(4, 3),
            Map::Point(-2, -1)
        )
    );
    auto rightAnswer = std::vector<Map::Point>{
        Map::Point(-2, -1),
        Map::Point(-1, -1),
        Map::Point( 0, -1),
        Map::Point( 1, -1),
        Map::Point(-2,  0),
        Map::Point(-1,  0),
        Map::Point( 0,  0),
        Map::Point( 1,  0),
        Map::Point(-2,  1),
        Map::Point(-1,  1),
        Map::Point( 0,  1),
        Map::Point( 1,  1),
    };
    auto rightAnswerIt = rightAnswer.begin();
    auto counter = size_t{0};
    while (iterator.isValid()) {
        UT_ASSERT_EQUAL(iterator.point(), *rightAnswerIt);
        UT_ASSERT_EQUAL(*iterator, *rightAnswerIt);
        ++iterator;
        ++rightAnswerIt;
        ++counter;
    }
    UT_ASSERT_EQUAL(counter, rightAnswer.size());
}

void squareIteratorConstexprTest() {
    std::cerr << " - squareIteratorConstexprTest\n";
    {
        constexpr auto it = ++Map::SquareIterator(
            Map::Square(
                Map::Vector(4, 3),
                Map::Point(0, 0)
            )
        );
        static_assert(it.point() == Map::Point(1, 0), "is suppose to be equal");
        static_assert(*it == Map::Point(1, 0), "is suppose to be equal");
        static_assert(it.isValid(), "is suppose to be valid");
    }
    {
        constexpr auto it = ++Map::SquareIterator(
            Map::Square(
                Map::Vector(1, 3),
                Map::Point(0, 0)
            )
        );
        static_assert(it.point() == Map::Point(0, 1), "is suppose to be equal");
        static_assert(*it == Map::Point(0, 1), "is suppose to be equal");
        static_assert(it.isValid(), "is suppose to be valid");
    }
    {
        constexpr auto it = --Map::SquareIterator(
            Map::Square(
                Map::Vector(2, 3),
                Map::Point(0, 0)
            ),
            Map::Point(1, 0)
        );
        static_assert(it.point() == Map::Point(0, 0), "is suppose to be equal");
        static_assert(*it == Map::Point(0, 0), "is suppose to be equal");
        static_assert(it.isValid(), "is suppose to be valid");
    }
    {
        constexpr auto it = --Map::SquareIterator(
            Map::Square(
                Map::Vector(1, 3),
                Map::Point(0, 0)
            )
        );
        static_assert(!it.isValid(), "is suppose to be not valid");
    }
    {
        constexpr auto it = ++Map::SquareIterator(
            Map::Square(
                Map::Vector(1, 1),
                Map::Point(0, 0)
            )
        );
        static_assert(!it.isValid(), "is suppose to be not valid");
    }
}

void fieldResizeTest() {
    std::cerr << " - fieldResizeTest\n";
    {
        std::string text = R"FieldMap(<3,3>
(0,0)
012
345
678
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(Map::Vector(4,4));
        std::string rightAnswer = R"FieldMap(<4,4>
(0,0)
012.
345.
678.
....
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(rightAnswer, out.str());
    }
    {
        std::string text = R"FieldMap(<2,2>
(0,0)
01
23
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(Map::Vector(3, 3), Map::Vector(1, 0));
        std::string rightAnswer = R"FieldMap(<3,3>
(-1,0)
.01
.23
...
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
    {
        std::string text = R"FieldMap(<3,3>
(0,0)
012
345
678
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(Map::Vector(4,4), Map::Vector(0, 1));
        std::string rightAnswer = R"FieldMap(<4,4>
(0,-1)
....
012.
345.
678.
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
    {
        std::string text = R"FieldMap(<3,3>
(0,0)
012
345
678
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(Map::Vector(4,4), Map::Vector(1, 1));
        std::string rightAnswer = R"FieldMap(<4,4>
(-1,-1)
....
.012
.345
.678
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
    {
        std::string text = R"FieldMap(<3,3>
(0,0)
012
345
678
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(field.size(), Map::Vector(1, 1));
        std::string rightAnswer = R"FieldMap(<3,3>
(-1,-1)
...
.01
.34
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
    {
        std::string text = R"FieldMap(<3,3>
(0,0)
012
345
678
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(field.size(), Map::Vector(-1, -1));
        std::string rightAnswer = R"FieldMap(<3,3>
(1,1)
45.
78.
...
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
    {
        std::string text = R"FieldMap(<3,3>
(0,0)
012
345
678
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(Map::Vector(2, 3));
        std::string rightAnswer = R"FieldMap(<2,3>
(0,0)
01
34
67
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
    {
        std::string text = R"FieldMap(<3,3>
(0,0)
012
345
678
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        field.resize(Map::Vector(1, 4), Map::Vector(-2, -1));
        std::string rightAnswer = R"FieldMap(<1,4>
(2,1)
5
8
.
.
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
}

void fieldExtendForTest() {
    std::cerr << " - fieldExtendForTest\n";
    {
        auto field = Map::Field<Map::SimpleCell>(
            Map::Vector(2, 2),
            Map::Point(0, 0)
        );
        UT_ASSERT(field.inRange(Map::Point(1, 1)));
        const auto pt0 = Map::Point(2, 2);
        UT_ASSERT(!field.inRange(pt0));
        field.extendFor(pt0);
        UT_ASSERT(field.inRange(pt0));

        const auto pt1 = Map::Point(0, 22);
        UT_ASSERT(!field.inRange(pt1));
        field.extendFor(pt1);
        UT_ASSERT(field.inRange(pt1));
        UT_ASSERT(!field.inRange(Map::Point(10, 22)));
    }
    {
        auto field = Map::Field<Map::SimpleCell>(
            Map::Vector(2, 2),
            Map::Point(0, 0)
        );
        const auto pt = Map::Point(-1, 1);
        UT_ASSERT(!field.inRange(pt));
        field.extendFor(pt);
        UT_ASSERT(field.inRange(pt));
    }
    {
        std::string text = R"FieldMap(<2,2>
(0,0)
01
23
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::SimpleCell>(in);
        const auto pt = Map::Point(-1, -1);
        UT_ASSERT(!field.inRange(pt));
        field.extendFor(pt);
        UT_ASSERT(field.inRange(pt));
        std::string rightAnswer = R"FieldMap(<4,4>
(-2,-2)
....
....
..01
..23
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        UT_ASSERT_EQUAL(out.str(), rightAnswer);
    }
}

int main(int argn, char** argv) {
    try {
        std::cerr << "2d_field_ut:\n";
        vectorAbsTest();
        vectorInverseTest();
        vectorNormalizeTest();
        pointStreamIO();
        vectorStreamIO();
        inRangeFieldTest();
        scanAndPrintField();
        compareTest();
        squareIteratorTest();
        squareIteratorConstexprTest();
        fieldResizeTest();
        fieldExtendForTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
