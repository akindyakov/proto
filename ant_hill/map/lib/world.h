#include "2d_field.h"


namespace Map {

class ObjectId
{
public:
    using Type = std::uint32_t;
    Type id;

private:
    static constexpr InvalidId_ = Type{0};

public:
    explicit constexpr ObjectId(
        Type id_
    ) noexcept
        : id(id_)
    {
    }

    constexpr const ObjectId Invalid() noexcept {
        return ObjectId(InvalidId_);
    }
};

struct WorldCell {
    explicit WorldCell() = default;

    explicit WorldCell(
        EMaterial::EmptySpace grain_
    )
        : grain(grain_)
    {
    }

    explicit WorldCell(
        char grain_
    )
        : grain(GetSymbolMap().GetMaterial(grain_))
    {
    }

    char ToChar() const {
        return GetSymbolMap().GetSymbol(grain);
    }

    Grain grain = EMaterial::EmptySpace;
    ObjectId objectId = ObjectId::Invalid();
};

class World {
private:
    Field<WorldCell> field_;
    std::map<ObjectId, std::shared_ptr<IObject>> objects_;
};

std::ostream& operator<<(std::ostream& os, const Map::Point& pt);
std::ostream& operator<<(std::ostream& os, const Map::Vector& vect);
std::istream& operator>>(std::istream& is, Map::Point& pt);
std::istream& operator>>(std::istream& is, Map::Vector& vect);

}  // namespace Map
