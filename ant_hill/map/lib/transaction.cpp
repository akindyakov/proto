#include "transaction.h"

#include <tools/tests/ut.h>


namespace Field {

Direction Direction::Diff(const Point& to, const Point& from) {
    auto shift = to - from;
    if (shift.X != 0 && shift.Y != 0) {
        throw AntHill::Exception("Direction move must be only horisontal or vertical");
    } else if (shift.X > 0) {
        return Direction::East;
    } else if (shift.X < 0) {
        return Direction::West;
    } else if (shift.Y > 0) {
        return Direction::North;
    }
    return Direction::South;
}

Movement::Movement(const Point& to, const Point& from)
    : To(to)
    , From(from)
{
}

Movement::Movement(Movement&& other)
    : To(std::move(other.To))
    , From(std::move(other.From))
{
}

Movement& Movement::operator=(Movement&& other) {
    To = std::move(other.To);
    From = std::move(other.From);
    return *this;
}

MoveTransaction::MoveTransaction(
    const std::vector<ShortMovement>& movement
) {
    for (const auto& m : movement) {
        Add(m.point_, m.direction_);
    }
}

MoveTransaction& MoveTransaction::Add(const Point& old, Direction direction) {
    auto action = Movement(MovePoint(old, direction), old);
    // std::cerr << "From: " << action.From.X << ", " << action.From.Y << std::endl;
    // std::cerr << "To: " << action.To.X << ", " << action.To.Y << std::endl;
    Actions.push_back(std::move(action));
    return *this;
}

bool MoveTransaction::Apply(Field& where) const {
    // ODO: there have to be full lock on the all poins of transaction!
    for (const auto& action : Actions) {
        auto& fromCell = where.At(action.From);
        auto& toCell = where.At(action.To);

        // std::cerr << "EMaterial::EmptySpace: " << int(EMaterial::EmptySpace) << std::endl;
        // std::cerr << "Material: " << int(toCell.Grain.SeeMaterial()) << std::endl;
        // std::cerr << "toCell.Grain.IsNone(): " << toCell.Grain.IsNone() << std::endl;

        if (fromCell.Grain.IsNone()) {
            throw AntHill::Exception("Expected error was not threw");
        }
        if (!toCell.Grain.IsNone()) {
            return false;
        }
        std::swap(fromCell.Grain, toCell.Grain);
    }
    return true;
}

//AppearanceTransaction& AppearanceTransaction::Add(
//    const Point& pt
//    , EMaterial material
//) {
//    Cells.emplace_back(material, pt);
//    return *this;
//}

Point AppearanceTransaction::Apply(Field& where) {
    auto start = Point{0, 0};
    for (auto x = where.min().X; x < where.max().X; ++x) {
        for (auto y = where.min().Y; y < where.max().Y; ++y) {
            start = Point{x, y};
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
    throw AntHill::Exception("There is no vacant position");
}

}  // Field
