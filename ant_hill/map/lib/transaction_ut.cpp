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
    auto field = Field::ScanFromText(in);
    {
        auto move = std::vector<Field::ShortMovement>{
            Field::ShortMovement{{2, 3}, Field::Direction::South},
            Field::ShortMovement{{2, 4}, Field::Direction::South},
            Field::ShortMovement{{3, 3}, Field::Direction::South},
            Field::ShortMovement{{3, 4}, Field::Direction::South},
        };
        auto lawsMove = Field::MoveTransaction{move};
        if (!lawsMove.Apply(field)) {
            throw AntHill::Exception("Error in move 1");
        }
    }
    {
        auto move = std::vector<Field::ShortMovement>{
            Field::ShortMovement({7, 2}, Field::Direction::East),
            Field::ShortMovement({7, 3}, Field::Direction::South),
            Field::ShortMovement({7, 4}, Field::Direction::South),
        };
        auto smiMove = Field::MoveTransaction{move};
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
    Field::PrintToText(out, field);
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
    auto field = Field::ScanFromText(in);

    {
        auto move = Field::MoveTransaction{};
        move
            .Add({1, 1}, Field::Direction::East)
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
    auto field = Field::ScanFromText(in);

    {
        auto appearance = Field::AppearanceTransaction{};
        appearance
            .Add(
                Field::ChainNode<EMaterial>(
                    EMaterial::AntBody
                )
            )
            .Add(
                Field::ChainNode<EMaterial>(
                    EMaterial::AntHead,
                    Field::Direction::North
                )
            )
        ;
        auto start = appearance.Apply(field);
        if (start != Field::Point{3, 3}) {
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
    Field::PrintToText(out, field);
    auto outText = out.str();
    if (endText != outText) {
        throw AntHill::Exception()
            << "Expected: " << endText
            << "\nGot: " << outText;
    }
}

void DirecitionTest() {
    Field::Direction direction = Field::Direction::North;
    if (direction != Field::Direction::North) {
        throw AntHill::Exception()
            << "Wrong not equal operator work";
    }
    if (
        direction == Field::Direction::West
        || direction == Field::Direction::South
        || direction == Field::Direction::East
    ) {
        throw AntHill::Exception()
            << "Wrong equal operator work";
    }
    if (direction.Inverse() != Field::Direction::South) {
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
