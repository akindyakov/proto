#include "snake.h"

#include <lib/tests/assert.h>

#include <iostream>
#include <sstream>
#include <string>

void frontMoveTest() {
    std::cerr << " - frontMoveTest\n";
    auto tail = std::vector<Map::Direction>{
        Map::Direction::North(),
    };
    auto head = Map::Point{0, 1};
    std::string text = R"FieldMap(<5,4>
(0,0)
#....
X....
.....
.....
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto ant = Map::SnakeObj<decltype(field)>(head, tail);

    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Right());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Right());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Right());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);

    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    std::string rightAnswer = R"FieldMap(<5,4>
(0,0)
X#...
.....
.....
.....
)FieldMap";
    UT_ASSERT_EQUAL(rightAnswer, out.str());
}

void backMoveTest() {
    std::cerr << " - backMoveTest\n";
    auto tail = std::vector<Map::Direction>{
        Map::Direction::North(),
    };
    auto head = Map::Point{0, 1};
    std::string text = R"FieldMap(<5,4>
(0,0)
#....
X....
.....
.....
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto ant = Map::SnakeObj<decltype(field)>(head, tail);

    //**/ Map::PrintToText(std::cerr, field);
    ant.backMove(field, Map::RelativeDirection::Right());
    //**/ Map::PrintToText(std::cerr, field);
    ant.backMove(field, Map::RelativeDirection::Backward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.backMove(field, Map::RelativeDirection::Backward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.backMove(field, Map::RelativeDirection::Backward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.backMove(field, Map::RelativeDirection::Right());
    //**/ Map::PrintToText(std::cerr, field);
    ant.backMove(field, Map::RelativeDirection::Backward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.backMove(field, Map::RelativeDirection::Backward());
    //**/ Map::PrintToText(std::cerr, field);

    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    std::string rightAnswer = R"FieldMap(<5,4>
(0,0)
.....
.....
....X
....#
)FieldMap";
    UT_ASSERT_EQUAL(rightAnswer, out.str());
}

void pickUpAndDropTest() {
    std::cerr << " - pickUpAndDropTest\n";
    auto tail = std::vector<Map::Direction>{
        Map::Direction::North(),
    };
    auto head = Map::Point{0, 1};
    std::string text = R"FieldMap(<5,4>
(0,0)
#....
Xw...
.....
.....
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto ant = Map::SnakeObj<decltype(field)>(head, tail);
    //**/ Map::PrintToText(std::cerr, field);
    auto pt = ant.pushFrontGrain(Map::RelativeDirection::Right());
    //**/ Map::PrintToText(std::cerr, field);
    UT_ASSERT_EQUAL(pt, Map::Point(1, 1));

    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);

    pt = ant.popFrontGrain();
    UT_ASSERT_EQUAL(pt, Map::Point(4, 1));

    ant.frontMove(field, Map::RelativeDirection::Left());
    //**/ Map::PrintToText(std::cerr, field);

    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);

    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    std::string rightAnswer = R"FieldMap(<5,4>
(0,0)
.....
....w
...#.
...X.
)FieldMap";
    UT_ASSERT_EQUAL(rightAnswer, out.str());
}

void lookToTest() {
    std::cerr << " - lookToTest\n";
    auto tail = std::vector<Map::Direction>{
        Map::Direction::North(),
    };
    auto head = Map::Point{1, 2};
    std::string text = R"FieldMap(<3,4>
(0,0)
.w.
m#a
iXl
?.s
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto ant = Map::SnakeObj<decltype(field)>(head, tail);
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Forward(), 0);
        UT_ASSERT_EQUAL(pt, Map::Point(1, 3));
    }
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Left(), 0);
        UT_ASSERT_EQUAL(pt, Map::Point(0, 2));
    }
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Right(), 0);
        UT_ASSERT_EQUAL(pt, Map::Point(2, 2));
    }
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Backward(), 0);
        UT_ASSERT_EQUAL(pt, Map::Point(1, 1));
    }
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Forward(), 1);
        UT_ASSERT_EQUAL(pt, Map::Point(1, 2));
    }
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Left(), 1);
        UT_ASSERT_EQUAL(pt, Map::Point(0, 1));
    }
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Right(), 1);
        UT_ASSERT_EQUAL(pt, Map::Point(2, 1));
    }
    {
        auto pt = ant.lookTo(Map::RelativeDirection::Backward(), 1);
        UT_ASSERT_EQUAL(pt, Map::Point(1, 0));
    }
}

void getBodyTest() {
    std::cerr << " - getBodyTest\n";
    auto tail = std::vector<Map::Direction>{
        Map::Direction::North(),
    };
    auto head = Map::Point{1, 2};
    std::string text = R"FieldMap(<3,4>
(0,0)
.w.
m#a
iXl
?.s
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto ant = Map::SnakeObj<decltype(field)>(head, tail);

    auto pt = ant.pushFrontGrain(Map::RelativeDirection::Left());
    UT_ASSERT_EQUAL(pt, Map::Point(0, 2));

    auto body = ant.getBody();
    UT_ASSERT_EQUAL(size_t{3}, body.size());
    UT_ASSERT_EQUAL(body[0], Map::Point(0, 2));
    UT_ASSERT_EQUAL(body[1], Map::Point(1, 2));
    UT_ASSERT_EQUAL(body[2], Map::Point(1, 1));
}

int main(int argn, char** argv) {
    try {
        std::cerr << "snake_ut:\n";
        frontMoveTest();
        backMoveTest();
        pickUpAndDropTest();
        lookToTest();
        getBodyTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed:\n";
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
