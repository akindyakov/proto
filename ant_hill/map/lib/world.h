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

    bool isFree() const {
        return grain == EMaterial::EmptySpace;
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
    virtual void frontMove(
        World::Field& field
        , Map::Direction frontDirection
    ) override;

    /**
    * Move back to specified direction
    */
    virtual void backMove(
        World::Field& field
        , Map::Direction backDirection
    ) override;

    /**
    * Add one more point to the front
    */
    virtual void pushFrontGrain(
        World::Field& field
        , Map::Direction frontDirection
    ) override;

    void popFrontGrain(
        World::Field& field
    ) override;

    /**
    * Add one more point to the back
    */
    void pushBackGrain(
        World::Field& field
        , Map::Direction backDirection
    ) override;

    void popBackGrain(
        World::Field& field
    ) override;

    void appear(
        World::Field& field
        , std::vector<Map::RelativeDirection>
    ) override;

    void look(
        World::Field& field
        , Map::RelativeDirection
        , size_t segment = 0
    ) const override;
};


}  // namespace Map

std::ostream& operator<<(std::ostream& os, const Map::WorldCell& cell);
std::istream& operator>>(std::istream& is, Map::WorldCell& cell);
