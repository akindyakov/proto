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
public:
    using Field = Field<WorldCell>;

private:
    Field field_;
    std::map<ObjectId, std::shared_ptr<IObject>> objects_;
};

class IObject
{
public:
    /**
    * Move head to specified direction
    */
    void FrontMove(
        World::Field& field
        , Map::Direction direction
    ) = 0;

    /**
    * Move back to specified direction
    */
    void BackMove(
        World::Field& field
        , Map::Direction direction
    ) = 0;

    /**
    * Add one more point to the front
    */
    void PushFrontGrain(
        World::Field& field
        , Map::Direction direction
    ) = 0;
    void PopFrontGrain(
        World::Field& field
        , Map::Direction direction
    ) = 0;

    /**
    * Add one more point to the back
    */
    void PushBackGrain(
        World::Field& field
        , Map::Direction direction
    ) = 0;
    void PopBackGrain(
        World::Field& field
        , Map::Direction direction
    ) = 0;
};


}  // namespace Map

std::ostream& operator<<(std::ostream& os, const Map::WorldCell& cell);
std::istream& operator>>(std::istream& is, Map::WorldCell& cell);
