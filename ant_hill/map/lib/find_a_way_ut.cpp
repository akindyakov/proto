#include "find_a_way.h"

#include <tools/tests/assert.h>

#include <iostream>
#include <sstream>
#include <string>


void findASimpleWayTest() {
    std::cerr << " - findASimpleWayTest\n";
    std::string text = R"FieldMap(<5,5>
(0,0)
.....
.....
..sss
.....
.....
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto simpleCost = [&field](const Map::Point& pt) {
        return (
            field.at(pt).grain == Map::EMaterial::EmptySpace
            ? 1
            : -1
        );
    };
    const auto start = Map::Point(3, 4);
    const auto finish = Map::Point(3, 0);
    auto curve = findFreeWay(
        start,
        finish,
        field.size(),
        field.min(),
        simpleCost
    );
    //*dbg*/ std::cerr << curve.size() << std::endl;
    auto pt = start;
    field.at(pt).grain = Map::EMaterial::AntHead;
    for (const auto& to : curve) {
        pt = to.MovePoint(pt);
        UT_ASSERT_EQUAL(field.at(pt).grain, Map::EMaterial::EmptySpace);
        field.at(pt).grain = Map::EMaterial::AntHead;
    }
    //*dbg*/ Map::PrintToText(std::cerr, field);
    UT_ASSERT_EQUAL(pt, finish);
}

void findAWayTest() {
    std::cerr << " - findAWayTest\n";
    std::string text = R"FieldMap(<8,8>
(0,0)
.......w
.s....w.
..s.....
...sw...
...wa...
..w..s..
.w....s.
.......s
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto simpleCost = [&field](const Map::Point& pt) {
        return (
            field.at(pt).grain == Map::EMaterial::EmptySpace
            ? 1
            : -1
        );
    };
    const auto start = Map::Point(7, 5);
    const auto finish = Map::Point(3, 5);
    auto curve = findFreeWay(
        start,
        finish,
        field.size(),
        field.min(),
        simpleCost
    );
    //*dbg*/ std::cerr << curve.size() << std::endl;
    auto pt = start;
    field.at(pt).grain = Map::EMaterial::AntHead;
    for (const auto& to : curve) {
        pt = to.MovePoint(pt);
        UT_ASSERT_EQUAL(field.at(pt).grain, Map::EMaterial::EmptySpace);
        field.at(pt).grain = Map::EMaterial::AntHead;
    }
    //*dbg*/ Map::PrintToText(std::cerr, field);
    UT_ASSERT_EQUAL(pt, finish);
}

void findSmthOnFieldTest() {
    std::cerr << " - findSmthOnTheFieldTest\n";
    std::string text = R"FieldMap(<8,8>
(0,0)
.......w
.s....w.
..s.....
...sw...
...wa...
..w..s..
.w....s.
.......s
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto simpleCost = [&field](const Map::Point& pt) {
        return (
            field.at(pt).grain == Map::EMaterial::EmptySpace
            ? 1
            : -1
        );
    };
    const auto start = Map::Point(7, 5);
    const auto finish = Map::Point(3, 5);
    auto check = [&finish](const Map::Point& pt) {
        return pt == finish;
    };
    auto curve = Map::findSmthOnTheField(
        start,
        field.size(),
        field.min(),
        simpleCost,
        check
    );
    //*dbg*/ std::cerr << curve.size() << std::endl;
    auto pt = start;
    field.at(pt).grain = Map::EMaterial::AntHead;
    for (const auto& to : curve) {
        pt = to.MovePoint(pt);
        UT_ASSERT_EQUAL(field.at(pt).grain, Map::EMaterial::EmptySpace);
        field.at(pt).grain = Map::EMaterial::AntHead;
    }
    //*dbg*/ Map::PrintToText(std::cerr, field);
    UT_ASSERT_EQUAL(pt, finish);
}

void squaresFinderTest() {
    std::cerr << " - squaresFinderTest\n";
    std::string text = R"FieldMap(<10,10>
(0,0)
s...ssssss
s...ss...s
s...ss...s
s........s
s........s
ssssssssss
...sssssss
...sssssss
s....s...s
ssssss...s
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::SimpleCell>(in);
    auto check = [&field](const Map::Point& pt) {
        return field.at(pt).grain == Map::EMaterial::EmptySpace;
    };
    auto finder = Map::makeSquaresFinder(
        field.min(),
        field.max(),
        check
    );
    auto answer = std::vector<Map::Square>{};
    auto rightAnswer = std::vector<Map::Square>{
        Map::Square(Map::Vector(2,4), Map::Point(1,0)),
        Map::Square(Map::Vector(1,1), Map::Point(4,3)),
        Map::Square(Map::Vector(2,3), Map::Point(6,1)),
        Map::Square(Map::Vector(2,1), Map::Point(0,6)),
        Map::Square(Map::Vector(3,0), Map::Point(1,8)),
        Map::Square(Map::Vector(2,1), Map::Point(6,8)),
    };
    for (const auto& rightSquare : rightAnswer) {
        auto sq = finder.next();
        UT_ASSERT_NOT_EQUAL(sq, nullptr);
        UT_ASSERT_EQUAL(*sq, rightSquare);
        answer.push_back(*sq);
    }
    UT_ASSERT_EQUAL(finder.next(), nullptr);

}

int main() {
    try {
        std::cerr << "find_a_way_ut:\n";
        findASimpleWayTest();
        findAWayTest();
        findSmthOnFieldTest();
        squaresFinderTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "Failed: " << except.what() << std::endl;
        return 1;
    }
}
