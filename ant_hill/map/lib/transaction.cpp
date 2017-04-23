#include "transaction.h"

#include <tools/tests/ut.h>


namespace NField {

Direction Direction::Diff(const TPoint& to, const TPoint& from) {
    auto shift = to - from;
    if (shift.X != 0 && shift.Y != 0) {
        throw NAntHill::TException("Direction move must be only horisontal or vertical");
    } else if (shift.X > 0) {
        return Direction::East;
    } else if (shift.X < 0) {
        return Direction::West;
    } else if (shift.Y > 0) {
        return Direction::North;
    }
    return Direction::South;
}

TMovement::TMovement(const TPoint& to, const TPoint& from)
    : To(to)
    , From(from)
{
}

TMovement::TMovement(TMovement&& other)
    : To(std::move(other.To))
    , From(std::move(other.From))
{
}

TMovement& TMovement::operator=(TMovement&& other) {
    To = std::move(other.To);
    From = std::move(other.From);
    return *this;
}

TMoveTransaction::TMoveTransaction(
    const std::vector<ShortMovement>& movement
) {
    for (const auto& m : movement) {
        Add(m.point_, m.direction_);
    }
}

TMoveTransaction& TMoveTransaction::Add(const TPoint& old, Direction direction) {
    auto action = TMovement(MovePoint(old, direction), old);
    // std::cerr << "From: " << action.From.X << ", " << action.From.Y << std::endl;
    // std::cerr << "To: " << action.To.X << ", " << action.To.Y << std::endl;
    Actions.push_back(std::move(action));
    return *this;
}

bool TMoveTransaction::Apply(TField& where) const {
    // TODO: there have to be full lock on the all poins of transaction!
    for (const auto& action : Actions) {
        auto& fromCell = where.At(action.From);
        auto& toCell = where.At(action.To);

        // std::cerr << "EMaterial::EmptySpace: " << int(EMaterial::EmptySpace) << std::endl;
        // std::cerr << "Material: " << int(toCell.Grain.SeeMaterial()) << std::endl;
        // std::cerr << "toCell.Grain.IsNone(): " << toCell.Grain.IsNone() << std::endl;

        if (fromCell.Grain.IsNone()) {
            throw NAntHill::TException("Expected error was not threw");
        }
        if (!toCell.Grain.IsNone()) {
            return false;
        }
        std::swap(fromCell.Grain, toCell.Grain);
    }
    return true;
}

//TAppearanceTransaction& TAppearanceTransaction::Add(
//    const TPoint& pt
//    , EMaterial material
//) {
//    Cells.emplace_back(material, pt);
//    return *this;
//}

TPoint TAppearanceTransaction::Apply(TField& where) {
    auto start = TPoint{0, 0};
    for (auto x = where.min().X; x < where.max().X; ++x) {
        for (auto y = where.min().Y; y < where.max().Y; ++y) {
            start = TPoint{x, y};
            bool vacant = true;
            {
                auto pt = start;
                for (auto& cell : Chain) {
                    pt = MovePoint(pt, cell.from);
                    if (!where.At(pt).Grain.IsNone()) {
                        vacant = false;
                        break;
                    }
                }
            }
            if (vacant) {
                auto pt = start;
                for (auto& cell : Chain) {
                    pt = MovePoint(pt, cell.from);
                    where.Insert(pt, cell.value);
                }
                return start;
            }
        }
    }
    throw NAntHill::TException("There is no vacant position");
}

}  // NField
