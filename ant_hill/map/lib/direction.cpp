#include "direction.h"

#include <tools/tests/ut.h>


namespace Map {

Direction Direction::Diff(const Point& to, const Point& from) {
    auto shift = to - from;
    if (shift.X != 0 && shift.Y != 0) {
        throw Exception("Direction move must be only horisontal or vertical");
    } else if (shift.X > 0) {
        return Direction::East();
    } else if (shift.X < 0) {
        return Direction::West();
    } else if (shift.Y > 0) {
        return Direction::North();
    }
    return Direction::South();
}

Movement::Movement(const Point& to, const Point& from)
    : To(to)
    , From(from)
{
}

RelativeDirectionCurve CurveToRelative(
    const DirectionCurve& dc
    , Direction start
) {
    auto ret = RelativeDirectionCurve{};
    auto prev = start;
    for (const auto& cur : dc) {
        ret.push_back(cur - prev);
        prev = cur;
    }
    return ret;
}

DirectionCurve RelativeCurveToCurve(
    const RelativeDirectionCurve& rdc
    , Direction base
) {
    auto ret = DirectionCurve{};
    for (const auto& rd : rdc) {
        base = rd.Turn(base);
        ret.push_back(base);
    }
    return ret;
}

// MoveTransaction::MoveTransaction(
//     const std::vector<ShortMovement>& movement
// ) {
//     for (const auto& m : movement) {
//         Add(m.point_, m.direction_);
//     }
// }
// 
// MoveTransaction& MoveTransaction::Add(const Point& old, Direction direction) {
//     auto action = Movement(direction.MovePoint(old), old);
//     // std::cerr << "From: " << action.From.X << ", " << action.From.Y << std::endl;
//     // std::cerr << "To: " << action.To.X << ", " << action.To.Y << std::endl;
//     Actions.push_back(std::move(action));
//     return *this;
// }
// 
// bool MoveTransaction::Apply(Field& where) const {
//     // ODO: there have to be full lock on the all poins of transaction!
//     for (const auto& action : Actions) {
//         auto& fromCell = where.At(action.From);
//         auto& toCell = where.At(action.To);
// 
//         // std::cerr << "EMaterial::EmptySpace: " << int(EMaterial::EmptySpace) << std::endl;
//         // std::cerr << "Material: " << int(toCell.Grain.SeeMaterial()) << std::endl;
//         // std::cerr << "toCell.Grain.IsNone(): " << toCell.Grain.IsNone() << std::endl;
// 
//         if (fromCell.Grain.IsNone()) {
//             throw Exception("Expected error was not threw");
//         }
//         if (!toCell.Grain.IsNone()) {
//             return false;
//         }
//         std::swap(fromCell.Grain, toCell.Grain);
//     }
//     return true;
// }
// 
// Point AppearanceTransaction::Apply(Field& where) {
//     auto start = Point{0, 0};
//     for (auto y = where.min().Y; y < where.max().Y; ++y) {
//         for (auto x = where.min().X; x < where.max().X; ++x) {
//             start = Point{x, y};
//             bool vacant = true;
//             {
//                 auto pt = start;
//                 for (auto& cell : this->chain_) {
//                     pt = cell.from.MovePoint(pt);
//                     if (!where.At(pt).Grain.IsNone()) {
//                         vacant = false;
//                         break;
//                     }
//                 }
//             }
//             if (vacant) {
//                 auto pt = start;
//                 for (auto& cell : this->chain_) {
//                     pt = cell.from.MovePoint(pt);
//                     where.Insert(pt, cell.value);
//                 }
//                 return start;
//             }
//         }
//     }
//     throw Exception("There is no vacant position");
// }


std::ostream& operator<<(std::ostream& os, const Map::Direction& adir) {
    os << adir.compass_;
    return os;
}
std::istream& operator>>(std::istream& is, Map::Direction& adir) {
    is >> adir.compass_;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Map::RelativeDirection& rdir) {
    os << rdir.rdir_;
    return os;
}
std::istream& operator>>(std::istream& is, Map::RelativeDirection& rdir) {
    is >> rdir.rdir_;
    return is;
}

}  // namespace Map
