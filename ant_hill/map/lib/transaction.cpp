#include "transaction.h"

#include <tools/tests/ut.h>


namespace NField {

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
    auto action = TMovement(old, old);
    if (direction == ECompass::North) {
        action.To.Y += 1;
    } else if (direction == ECompass::West) {
        action.To.X -= 1;
    } else if (direction == ECompass::South) {
        action.To.Y -= 1;
    } else if (direction == ECompass::East) {
        action.To.X += 1;
    }
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
    for (TMeasure x = 0; x < where.GetXSize(); ++x) {
        bool vacant = true;
        for (auto& cell : Cells) {
            if (!where.At(cell.Point).Grain.IsNone()) {
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
