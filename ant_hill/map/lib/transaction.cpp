#include "transaction.h"

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

TTransaction& TTransaction::Move(const TPoint& old, ECompass direction) {
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

bool TTransaction::Apply(TField& where) const {
    // TODO: there have to be full lock on the all poins of transaction!
    auto lock = std::vector<
        std::unique_lock<std::mutex>
    >{};
    for (const auto& action : Actions) {
        auto& fromCell = where.At(action.From);
        auto& toCell = where.At(action.To);

        std::unique_lock<std::mutex> lk1(fromCell.Mutex, std::defer_lock);
        std::unique_lock<std::mutex> lk2(toCell.Mutex, std::defer_lock);
        std::lock(lk1, lk2);

        if (!toCell.Grain.IsNone()) {
            return false;
        }
        std::swap(fromCell.Grain, toCell.Grain);
    }
    return true;
}

}  // NField
