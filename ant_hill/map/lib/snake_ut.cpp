#include "snake.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void appearTest() {
    auto chain = std::vector<Map::EMaterial>{
        Map::EMaterial::AntBody,
        Map::EMaterial::AntHead,
        Map::EMaterial::Water,
    };
    {
        std::string text = R"FieldMap(<5,4>
(0,0)
.....
.....
.....
.....
.....
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::WorldCell>(in);
        Map::SnakeObj::appear(
            field,
            chain,
            Map::ObjectId(2)
        );
        std::string rightAnswer = R"FieldMap(<5,4>
(0,0)
#....
X....
a....
.....
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        ValidateEqual(rightAnswer, out.str());
    }
    {
        std::string text = R"FieldMap(<5,4>
(0,0)
iiiii
ii.ii
ii.ii
ii.ii
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::WorldCell>(in);
        Map::SnakeObj::appear(
            field,
            chain,
            Map::ObjectId(2)
        );
        std::string rightAnswer = R"FieldMap(<5,4>
(0,0)
iiiii
ii#ii
iiXii
iiaii
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        ValidateEqual(rightAnswer, out.str());
    }
}

void frontMoveTest() {
    auto chain = std::vector<Map::EMaterial>{
        Map::EMaterial::AntBody,
        Map::EMaterial::AntHead,
    };
    std::string text = R"FieldMap(<5,5>
(0,0)
.....
.....
.....
.....
.....
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::WorldCell>(in);
    auto ant = Map::SnakeObj::appear(
        field,
        chain,
        Map::ObjectId(2)
    );
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
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

    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    std::string rightAnswer = R"FieldMap(<5,5>
(0,0)
.....
.....
.....
....X
....#
)FieldMap";
    ValidateEqual(rightAnswer, out.str());
}

void backMoveTest() {
    auto chain = std::vector<Map::EMaterial>{
        Map::EMaterial::AntBody,
        Map::EMaterial::AntHead,
    };
    std::string text = R"FieldMap(<5,4>
(0,0)
.....
.....
.....
.....
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::WorldCell>(in);
    auto ant = Map::SnakeObj::appear(
        field,
        chain,
        Map::ObjectId(2)
    );
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
    ValidateEqual(rightAnswer, out.str());
}

void pickUpAndDropTest() {
    auto chain = std::vector<Map::EMaterial>{
        Map::EMaterial::AntBody,
        Map::EMaterial::AntHead,
    };
    std::string text = R"FieldMap(<5,4>
(0,0)
.....
.w...
.....
.....
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::WorldCell>(in);
    auto antId = Map::ObjectId(2);
    auto ant = Map::SnakeObj::appear(
        field,
        chain,
        antId
    );
    ValidateEqual(
        field.at({1, 1}).objectId,
        Map::ObjectId::Invalid()
    );
    //**/ Map::PrintToText(std::cerr, field);
    ant.pushFrontGrain(field, Map::RelativeDirection::Right());
    //**/ Map::PrintToText(std::cerr, field);

    ValidateEqual(
        field.at({1, 1}).objectId,
        antId
    );

    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);
    ant.frontMove(field, Map::RelativeDirection::Forward());
    //**/ Map::PrintToText(std::cerr, field);

    ValidateEqual(
        field.at({1, 1}).objectId,
        Map::ObjectId::Invalid()
    );

    ValidateEqual(
        field.at({4, 1}).objectId,
        antId
    );

    ant.popFrontGrain(field);

    ValidateEqual(
        field.at({4, 1}).objectId,
        Map::ObjectId::Invalid()
    );

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
    ValidateEqual(rightAnswer, out.str());
}

int main(int argn, char** argv) {
    try {
        appearTest();
        frontMoveTest();
        backMoveTest();
        pickUpAndDropTest();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
