#include "2d_field.h"
#include "transaction.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

void MoveTest() {
    std::string startText = R"FieldMap(10
10
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
    auto in = std::istringstream(startText);
    auto field = Map::ScanFromText(in);
    {
        auto move = std::vector<Map::ShortMovement>{
            Map::ShortMovement{{2, 3}, Map::Direction::South},
            Map::ShortMovement{{2, 4}, Map::Direction::South},
            Map::ShortMovement{{3, 3}, Map::Direction::South},
            Map::ShortMovement{{3, 4}, Map::Direction::South},
        };
        auto lawsMove = Map::MoveTransaction{move};
        if (!lawsMove.Apply(field)) {
            throw AntHill::Exception("Error in move 1");
        }
    }
    {
        auto move = std::vector<Map::ShortMovement>{
            Map::ShortMovement({7, 2}, Map::Direction::East),
            Map::ShortMovement({7, 3}, Map::Direction::South),
            Map::ShortMovement({7, 4}, Map::Direction::South),
        };
        auto smiMove = Map::MoveTransaction{move};
        if (!smiMove.Apply(field)) {
            throw AntHill::Exception("Error in move 2");
        }
    }
    std::string endText = R"FieldMap(10
10
iiiiiiiiii
i........i
i........i
i........i
i........i
i........i
i.la...s.i
i.ws...mii
i........i
iiiiiiiiii
)FieldMap";
    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    auto outText = out.str();
    if (endText != outText) {
        throw AntHill::Exception()
            << "Expected: " << endText
            << "\nGot: " << outText;
    }
}

void WrongMoveTest() {
    std::string startText = R"FieldMap(4
4
....
....
.wi.
....
)FieldMap";
    auto in = std::istringstream(startText);
    auto field = Map::ScanFromText(in);

    {
        auto move = Map::MoveTransaction{};
        move
            .Add({1, 1}, Map::Direction::East)
        ;
        if (move.Apply(field)) {
            throw AntHill::Exception("[WrongMoveTest] Expected error was not threw");
        }
    }
}

void YieldTest() {
    std::string startText = R"FieldMap(10
10
iiiiiiiiii
i........i
i........i
i........i
i........i
i........i
i........i
i.w......i
iww......i
iiiiiiiiii
)FieldMap";
    auto in = std::istringstream(startText);
    auto field = Map::ScanFromText(in);

    {
        auto appearance = Map::AppearanceTransaction{};
        appearance
            .Add(
                Map::ChainNode<Map::EMaterial>(
                    Map::EMaterial::AntBody
                )
            )
            .Add(
                Map::ChainNode<Map::EMaterial>(
                    Map::EMaterial::AntHead,
                    Map::Direction::North
                )
            )
        ;
        auto start = appearance.Apply(field);
        if (start != Map::Point{3, 3}) {
            throw AntHill::Exception("[YieldTest] Wrong shift.")
                << " Expected: [3, 3]"
                << "\nGot: [" << start.X << ", " << start.Y << "]";
        }
    }
    std::string endText = R"FieldMap(10
10
iiiiiiiiii
i........i
i........i
i........i
i........i
i..X.....i
i..#.....i
i.w......i
iww......i
iiiiiiiiii
)FieldMap";
    auto out = std::ostringstream();
    Map::PrintToText(out, field);
    auto outText = out.str();
    if (endText != outText) {
        throw AntHill::Exception()
            << "Expected: " << endText
            << "\nGot: " << outText;
    }
}

void DirecitionTest() {
    Map::Direction direction = Map::Direction::North;
    if (direction != Map::Direction::North) {
        throw AntHill::Exception()
            << "Wrong not equal operator work";
    }
    if (
        direction == Map::Direction::West
        || direction == Map::Direction::South
        || direction == Map::Direction::East
    ) {
        throw AntHill::Exception()
            << "Wrong equal operator work";
    }
    if (direction.Inverse() != Map::Direction::South) {
        throw AntHill::Exception()
            << "Wrong Inverse work";
    }
}

int main(int argn, char** argv) {
    try {
        MoveTest();
        YieldTest();
        WrongMoveTest();
        DirecitionTest();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
