#include "transaction.h"

#include <tools/tests/ut.h>


namespace NField {

ECompass DirectionDiff(const TPoint& to, const TPoint& from) {
    auto shift = to - from;
    if (shift.X != 0 && shift.Y != 0) {
        throw NAntHill::TException("Direction move must be only horisontal or vertical");
    } else if (shift.X > 0) {
        return ECompass::East;
    } else if (shift.X < 0) {
        return ECompass::West;
    } else if (shift.Y > 0) {
        return ECompass::North;
    }
    return ECompass::South;
}

TPoint MovePoint(TPoint pt, ECompass direction) {
    if (direction == ECompass::North) {
        pt.Y += 1;
    } else if (direction == ECompass::West) {
        pt.X -= 1;
    } else if (direction == ECompass::South) {
        pt.Y -= 1;
    } else if (direction == ECompass::East) {
        pt.X += 1;
    }
    return pt;
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

TMoveTransaction& TMoveTransaction::Add(const TPoint& old, ECompass direction) {
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

TAppearanceTransaction& TAppearanceTransaction::Add(
    const TPoint& pt
    , EMaterial material
) {
    Cells.emplace_back(material, pt);
    return *this;
}

TVector TAppearanceTransaction::Apply(TField& where) {
    auto shift = TVector{0, 0};
    auto shiftStep = TVector{1, 1};
    for (auto x = where.BottomLeft().X; x < where.TopRight().X; ++x) {
        bool vacant = true;
        for (auto& cell : Cells) {
            if (!where.At(cell.Point).Grain.IsNone()) {
                // std::cerr << "Check: " << cell.Point.X << ", " << cell.Point.Y << std::endl;
                vacant = false;
                break;
            }
        }
        if (vacant) {
            for (auto& cell : Cells) {
                where.Insert(cell.Point, cell.Material);
            }
            break;
        }
        for (auto& cell : Cells) {
            cell.Point += shiftStep;
        }
        shift += shiftStep;
    }
    return shift;
}

}  // NField
