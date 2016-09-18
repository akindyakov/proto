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
        auto lawsMove = NField::TTransaction{};
        lawsMove
            .Move({2, 3}, NField::ECompass::South)
            .Move({2, 4}, NField::ECompass::South)
            .Move({3, 3}, NField::ECompass::South)
            .Move({3, 4}, NField::ECompass::South)
        ;
        if (!lawsMove.Apply(field)) {
            throw NAntHill::TException("Error in move 1");
        }
    }
    {
        auto smiMove = NField::TTransaction{};
        smiMove
            .Move({7, 2}, NField::ECompass::East)
            .Move({7, 3}, NField::ECompass::South)
            .Move({7, 4}, NField::ECompass::South)
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
        auto move = NField::TTransaction{};
        move
            .Move({1, 1}, NField::ECompass::East)
        ;
        if (move.Apply(field)) {
            throw NAntHill::TException("Expected error was not threw");
        }
    }
}

int main(int argn, char** argv) {
    try {
        MoveTest();
        WrongMoveTest();
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
