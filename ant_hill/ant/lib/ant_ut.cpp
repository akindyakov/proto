#include "ant.h"

#include <tools/tests/ut.h>

#include <iostream>


void SnakeAntBodyMoveTest() {
    auto ant = NAnt::SnakeAntBody({1, 1}, {0, 1});
    auto diff = ant.DiffHeadMove(NField::Direction::North);
    auto rightDiff = std::vector<NField::ShortMovement>{
        NField::ShortMovement{{1, 1}, NField::Direction::North},
        NField::ShortMovement{{0, 1}, NField::Direction::East},
    };
    if (rightDiff != diff) {
        auto err = NAntHill::TException();
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
