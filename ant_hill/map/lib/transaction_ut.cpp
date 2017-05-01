#include "transaction.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <sstream>
#include <string>

// void MoveTest() {
//     std::string startText = R"FieldMap(10
// 10
// iiiiiiiiii
// i........i
// i........i
// i........i
// i........i
// i.la...s.i
// i.ws...m.i
// i......i.i
// i........i
// iiiiiiiiii
// )FieldMap";
//     auto in = std::istringstream(startText);
//     auto field = Map::ScanFromText(in);
//     {
//         auto move = std::vector<Map::ShortMovement>{
//             Map::ShortMovement{{2, 3}, Map::Direction::South},
//             Map::ShortMovement{{2, 4}, Map::Direction::South},
//             Map::ShortMovement{{3, 3}, Map::Direction::South},
//             Map::ShortMovement{{3, 4}, Map::Direction::South},
//         };
//         auto lawsMove = Map::MoveTransaction{move};
//         if (!lawsMove.Apply(field)) {
//             throw Exception("Error in move 1");
//         }
//     }
//     {
//         auto move = std::vector<Map::ShortMovement>{
//             Map::ShortMovement({7, 2}, Map::Direction::East),
//             Map::ShortMovement({7, 3}, Map::Direction::South),
//             Map::ShortMovement({7, 4}, Map::Direction::South),
//         };
//         auto smiMove = Map::MoveTransaction{move};
//         if (!smiMove.Apply(field)) {
//             throw Exception("Error in move 2");
//         }
//     }
//     std::string endText = R"FieldMap(10
// 10
// iiiiiiiiii
// i........i
// i........i
// i........i
// i........i
// i........i
// i.la...s.i
// i.ws...mii
// i........i
// iiiiiiiiii
// )FieldMap";
//     auto out = std::ostringstream();
//     Map::PrintToText(out, field);
//     auto outText = out.str();
//     if (endText != outText) {
//         throw Exception()
//             << "Expected: " << endText
//             << "\nGot: " << outText;
//     }
// }
// 
// void WrongMoveTest() {
//     std::string startText = R"FieldMap(4
// 4
// ....
// ....
// .wi.
// ....
// )FieldMap";
//     auto in = std::istringstream(startText);
//     auto field = Map::ScanFromText(in);
// 
//     {
//         auto move = Map::MoveTransaction{};
//         move
//             .Add({1, 1}, Map::Direction::East)
//         ;
//         if (move.Apply(field)) {
//             throw Exception("[WrongMoveTest] Expected error was not threw");
//         }
//     }
// }
// 
// void YieldTest() {
//     std::string startText = R"FieldMap(10
// 10
// iiiiiiiiii
// i........i
// i........i
// i........i
// i........i
// i........i
// i........i
// i.w......i
// iwww.....i
// iiiiiiiiii
// )FieldMap";
//     auto in = std::istringstream(startText);
//     auto field = Map::ScanFromText(in);
// 
//     {
//         auto appearance = Map::AppearanceTransaction{};
//         appearance
//             .Add(
//                 Map::ChainNode<Map::EMaterial>(
//                     Map::EMaterial::AntBody
//                 )
//             )
//             .Add(
//                 Map::ChainNode<Map::EMaterial>(
//                     Map::EMaterial::AntHead,
//                     Map::Direction::North
//                 )
//             )
//         ;
//         auto start = appearance.Apply(field);
//         if (start != Map::Point{4, 1}) {
//             throw Exception("[YieldTest] Wrong shift.")
//                 << " Expected: [4, 1]"
//                 << "\nGot: [" << start.X << ", " << start.Y << "]";
//         }
//     }
//     std::string endText = R"FieldMap(10
// 10
// iiiiiiiiii
// i........i
// i........i
// i........i
// i........i
// i........i
// i........i
// i.w.X....i
// iwww#....i
// iiiiiiiiii
// )FieldMap";
//     auto out = std::ostringstream();
//     Map::PrintToText(out, field);
//     auto outText = out.str();
//     if (endText != outText) {
//         throw Exception()
//             << "Expected: " << endText
//             << "\nGot: " << outText;
//     }
// }

// void DirecitionTest() {
//     Map::Direction direction = Map::Direction::North;
//     if (direction != Map::Direction::North) {
//         throw Exception()
//             << "Wrong not equal operator work";
//     }
//     if (
//         direction != Map::Direction::North
//         || direction == Map::Direction::South
//         || direction == Map::Direction::East
//         || direction == Map::Direction::West
//     ) {
//         throw Exception()
//             << "Wrong equal operator work";
//     }
//     if (direction.Inverse() != Map::Direction::South) {
//         throw Exception()
//             << "Wrong Inverse work";
//     }
// }

void DirectionInverse() {
    {
        auto dir = Map::Direction::North().Inverse();
        Validate(dir, Map::Direction::South());
    }
    {
        auto dir = Map::Direction::West().Inverse();
        Validate(dir, Map::Direction::East());
    }
    {
        auto dir = Map::Direction::South().Inverse();
        Validate(dir, Map::Direction::North());
    }
    {
        auto dir = Map::Direction::East().Inverse();
        Validate(dir, Map::Direction::West());
    }
}

void DirectionDiff() {
    Validate(
        Map::Direction::Diff({0, 1}, {0, 0}),
        Map::Direction::North()
    );
    Validate(
        Map::Direction::Diff({10, -99}, {10, -100}),
        Map::Direction::North()
    );
    Validate(
        Map::Direction::Diff({0, 0}, {1, 0}),
        Map::Direction::West()
    );
    Validate(
        Map::Direction::Diff({0, 0}, {0, 1}),
        Map::Direction::South()
    );
    Validate(
        Map::Direction::Diff({2, 0}, {0, 0}),
        Map::Direction::East()
    );
}

void DirectionMovePoint() {
    Validate(
        Map::Direction::East().MovePoint({1, 1}),
        Map::Point{2, 1}
    );
    Validate(
        Map::Direction::South().MovePoint({2, 2}),
        Map::Point{2, 1}
    );
    Validate(
        Map::Direction::West().MovePoint({3, 1}),
        Map::Point{2, 1}
    );
    Validate(
        Map::Direction::North().MovePoint({-4, -1}),
        Map::Point{-4, 0}
    );
}

void RelativeDirectionTurn() {
    {
        auto dir = Map::Direction::North();
        Validate(
            Map::RelativeDirection::Forward().Turn(dir),
            Map::Direction::North()
        );
        Validate(
            Map::RelativeDirection::Backward().Turn(dir),
            Map::Direction::South()
        );
        Validate(
            Map::RelativeDirection::Left().Turn(dir),
            Map::Direction::West()
        );
    }
    {
        auto dir = Map::Direction::East();
        Validate(
            Map::RelativeDirection::Forward().Turn(dir),
            Map::Direction::East()
        );
        Validate(
            Map::RelativeDirection::Backward().Turn(dir),
            Map::Direction::West()
        );
        Validate(
            Map::RelativeDirection::Right().Turn(dir),
            Map::Direction::South()
        );
        Validate(
            Map::RelativeDirection::Left().Turn(dir),
            Map::Direction::North()
        );
    }
}

int main(int argn, char** argv) {
    try {
        DirectionInverse();
        DirectionDiff();
        DirectionMovePoint();
        RelativeDirectionTurn();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
