#include "snake.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void AppearTest() {
    auto chain = Map::Chain<Map::RelativeDirection, Map::EMaterial>{
        {Map::RelativeDirection::Forward(), Map::EMaterial::AntBody},
        {Map::RelativeDirection::Forward(), Map::EMaterial::AntHead},
        {Map::RelativeDirection::Left(),    Map::EMaterial::Water},
    };
    {
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
        Map::SnakeObj::appear(
            field,
            chain,
            Map::ObjectId(2)
        );
        std::string rightAnswer = R"FieldMap(<5,5>
(0,0)
.#X..
..a..
.....
.....
.....
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        ValidateEqual(rightAnswer, out.str());
    }
    {
        std::string text = R"FieldMap(<5,5>
(0,0)
iiiii
ii..i
ii.ii
iiiii
iiiii
)FieldMap";
        auto in = std::istringstream(text);
        auto field = Map::ScanFromText<Map::WorldCell>(in);
        Map::SnakeObj::appear(
            field,
            chain,
            Map::ObjectId(2)
        );
        std::string rightAnswer = R"FieldMap(<5,5>
(0,0)
iiiii
iiXai
ii#ii
iiiii
iiiii
)FieldMap";
        auto out = std::ostringstream();
        Map::PrintToText(out, field);
        ValidateEqual(rightAnswer, out.str());
    }
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
