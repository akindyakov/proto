#include "ant.h"

#include <tools/tests/ut.h>

#include <iostream>


void SnakeAntBodyMoveTest() {
    auto ant = Ant::SnakeAntBody({1, 1}, {0, 1});
    auto diff = ant.DiffHeadMove(Map::Direction::North);
    auto rightDiff = std::vector<Map::ShortMovement>{
        Map::ShortMovement{{1, 1}, Map::Direction::North},
        Map::ShortMovement{{0, 1}, Map::Direction::East},
    };
    if (rightDiff != diff) {
        auto err = AntHill::Exception();
        err << "Wrong answer: ";
        for (const auto& d : diff) {
            err
                << "( " << d.point_.X << ", " << d.point_.Y
                << ", " << int(d.direction_.compass_) << " ) "
            ;
        }
        throw err;
    }
}

int main(int argn, char** argv) {
    try {
        SnakeAntBodyMoveTest();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
