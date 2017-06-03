#include "find_a_way.h"

#include <tools/tests/ut.h>

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
    /*dbg*/ std::cerr << curve.size() << std::endl;
    auto pt = start;
    field.at(pt).grain = Map::EMaterial::AntHead;
    for (const auto& to : curve) {
        pt = to.MovePoint(pt);
        ValidateEqual(field.at(pt).grain, Map::EMaterial::EmptySpace);
        field.at(pt).grain = Map::EMaterial::AntHead;
    }
    /*dbg*/ Map::PrintToText(std::cerr, field);
    ValidateEqual(pt, finish);
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
        ValidateEqual(field.at(pt).grain, Map::EMaterial::EmptySpace);
        field.at(pt).grain = Map::EMaterial::AntHead;
    }
    //*dbg*/ Map::PrintToText(std::cerr, field);
    ValidateEqual(pt, finish);
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
    /*dbg*/ std::cerr << curve.size() << std::endl;
    auto pt = start;
    field.at(pt).grain = Map::EMaterial::AntHead;
    for (const auto& to : curve) {
        pt = to.MovePoint(pt);
        ValidateEqual(field.at(pt).grain, Map::EMaterial::EmptySpace);
        field.at(pt).grain = Map::EMaterial::AntHead;
    }
    /*dbg*/ Map::PrintToText(std::cerr, field);
    ValidateEqual(pt, finish);
}

int main() {
    try {
        std::cerr << "find_a_way_ut:\n";
        findASimpleWayTest();
        findAWayTest();
        findSmthOnFieldTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "Failed: " << except.what() << std::endl;
        return 1;
    }
}
