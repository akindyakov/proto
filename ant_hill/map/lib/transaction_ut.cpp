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
    auto field = NField::ScanFromText(in);

    {
        auto lawsMove = NField::TMoveTransaction{};
        lawsMove
            .Add({2, 3}, NField::ECompass::South)
            .Add({2, 4}, NField::ECompass::South)
            .Add({3, 3}, NField::ECompass::South)
            .Add({3, 4}, NField::ECompass::South)
        ;
        if (!lawsMove.Apply(field)) {
            throw NAntHill::TException("Error in move 1");
        }
    }
    {
        auto smiMove = NField::TMoveTransaction{};
        smiMove
            .Add({7, 2}, NField::ECompass::East)
            .Add({7, 3}, NField::ECompass::South)
            .Add({7, 4}, NField::ECompass::South)
        ;
        if (!smiMove.Apply(field)) {
            throw NAntHill::TException("Error in move 2");
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
    NField::PrintToText(out, field);
    auto outText = out.str();
    if (endText != outText) {
        throw NAntHill::TException()
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
    auto field = NField::ScanFromText(in);

    {
        auto move = NField::TMoveTransaction{};
        move
            .Add({1, 1}, NField::ECompass::East)
        ;
        if (move.Apply(field)) {
            throw NAntHill::TException("[WrongMoveTest] Expected error was not threw");
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
    auto field = NField::ScanFromText(in);

    {
        auto appearance = NField::TAppearanceTransaction{};
        appearance
            .Add({0, 0}, EMaterial::AntBody)
            .Add({0, 1}, EMaterial::AntHead)
        ;
        auto shift = appearance.Apply(field);
        if (shift != NField::TVector{3, 3}) {
            throw NAntHill::TException("[YieldTest] Wrong shift.")
                << " Expected: [3, 3]"
                << "\nGot: [" << shift.X << ", " << shift.Y << "]";
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
    NField::PrintToText(out, field);
    auto outText = out.str();
    if (endText != outText) {
        throw NAntHill::TException()
            << "Expected: " << endText
            << "\nGot: " << outText;
    }
}

int main(int argn, char** argv) {
    try {
        MoveTest();
        YieldTest();
        WrongMoveTest();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
