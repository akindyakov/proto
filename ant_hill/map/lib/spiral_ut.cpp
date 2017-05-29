#include "spiral.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void spiralTest() {
    std::cerr << " - spiralTest\n";
    auto spiral = Map::Spiral{Map::Point{0, 0}};

    auto points = std::vector<Map::Point>{};
    auto curve = std::vector<Map::Direction>{};
    while (true) {
        auto pt = spiral.next();
        if (spiral.radius() > 2) {
            break;
        }
        points.push_back(pt);
        curve.push_back(spiral.direction());
    }
    auto rightPoints = std::vector<Map::Point>{
        // radius 1
        Map::Point( 0,  1),
        Map::Point(-1,  1),
        Map::Point(-1,  0),
        Map::Point(-1, -1),
        Map::Point( 0, -1),
        Map::Point( 1, -1),
        Map::Point( 1,  0),
        Map::Point( 1,  1),
        // radius 2
        Map::Point( 1, 2),
        Map::Point( 0, 2),
        Map::Point(-1, 2),
        Map::Point(-2, 2),
        Map::Point(-2, 1),
        Map::Point(-2, 0),
        Map::Point(-2,-1),
        Map::Point(-2,-2),
        Map::Point(-1,-2),
        Map::Point( 0,-2),
        Map::Point( 1,-2),
        Map::Point( 2,-2),
        Map::Point( 2,-1),
        Map::Point( 2, 0),
        Map::Point( 2, 1),
        Map::Point( 2, 2),
    };
    auto rightCurve = std::vector<Map::Direction>{
        // radius 1
        Map::Direction::West(),
        Map::Direction::West(),
        Map::Direction::South(),
        Map::Direction::South(),
        Map::Direction::East(),
        Map::Direction::East(),
        Map::Direction::North(),
        Map::Direction::North(),
        // radius 2
        Map::Direction::West(),
        Map::Direction::West(),
        Map::Direction::West(),
        Map::Direction::West(),
        Map::Direction::South(),
        Map::Direction::South(),
        Map::Direction::South(),
        Map::Direction::South(),
        Map::Direction::East(),
        Map::Direction::East(),
        Map::Direction::East(),
        Map::Direction::East(),
        Map::Direction::North(),
        Map::Direction::North(),
        Map::Direction::North(),
        Map::Direction::North(),
    };
    ValidateEqual(rightPoints.size(), points.size());
    ValidateEqual(rightCurve.size(), curve.size());
    for (size_t i = 0; i < rightPoints.size(); ++i) {
        ValidateEqual(rightPoints[i], points[i]);
        ValidateEqual(rightCurve[i], curve[i]);
    }
}

int main(int argn, char** argv) {
    try {
        std::cerr << "spiral_ut:\n";
        spiralTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
