#include "world.h"

#include <lib/tests/assert.h>

#include <iostream>
#include <vector>


void appearTest() {
    std::cerr << " - appearTest\n";
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
        Map::createSnakeOnField(
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
        UT_ASSERT_EQUAL(rightAnswer, out.str());
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
        Map::createSnakeOnField(
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
        UT_ASSERT_EQUAL(rightAnswer, out.str());
    }
}


int main() {
    try {
        std::cerr << "world_ut:\n";
        appearTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
