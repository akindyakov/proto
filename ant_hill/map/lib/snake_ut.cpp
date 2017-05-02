#include "snake.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void AppearTest() {
    std::string text = R"FieldMap(<10,10>
(0,0)
iiiiiiiiii
i........i
i........i
i........i
i........i
i.la...s.i
i.ws...m.i
i......i.i
i........i
iiiiiiiiii
)FieldMap";
    auto in = std::istringstream(text);
    auto field = Map::ScanFromText<Map::WorldCell>(in);
    //auto chain = Map::Chain<Map::RelativeDirection, Map::EMaterial>{};
    //chain.emplace_back(Map::RelativeDirection::Forward(), Map::EMaterial::AntBody);
    //chain.emplace_back(Map::RelativeDirection::Forward(), Map::EMaterial::AntHead);
    //chain.emplace_back(Map::RelativeDirection::Left(),    Map::EMaterial::Water); 
    auto chain = Map::Chain<Map::RelativeDirection, Map::EMaterial>{
        {Map::RelativeDirection::Forward(), Map::EMaterial::AntBody},
        {Map::RelativeDirection::Forward(), Map::EMaterial::AntHead},
        {Map::RelativeDirection::Left(),    Map::EMaterial::Water},
    };
    Map::SnakeObj::appear(
        field,
        chain,
        Map::ObjectId(2)
    );
    PrintToText(std::cerr, field);

}

int main(int argn, char** argv) {
    try {
        std::cerr << "snake ut\n";
        AppearTest();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
