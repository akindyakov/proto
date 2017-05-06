#include "2d_field.h"
#include "map_symbols.h"
#include "transaction.h"


namespace Map {

class ObjectId
{
public:
    using Type = std::uint32_t;
    Type id;

private:
    static constexpr auto InvalidId_ = Type{0};

public:
    explicit constexpr ObjectId(
        Type id_
    ) noexcept
        : id(id_)
    {
    }

    constexpr bool isValid() noexcept {
        return id != InvalidId_;
    }

    static constexpr const ObjectId Invalid() noexcept {
        return ObjectId(InvalidId_);
    }
};

inline bool operator == (
    const ObjectId& first
    , const ObjectId& second
) {
    return (
        first.id == second.id
    );
}

inline bool operator != (
    const ObjectId& first
    , const ObjectId& second
) {
    return (
        first.id != second.id
    );
}

struct WorldCell {
    explicit WorldCell() = default;

    explicit WorldCell(
        EMaterial grain_
    )
        : grain(grain_)
    {
    }

    bool isFree() const {
        return grain == EMaterial::EmptySpace;
    }

    EMaterial grain = EMaterial::EmptySpace;
    ObjectId objectId = ObjectId::Invalid();
};

class IObject;

class World {
public:
    using Field = Field<WorldCell>;

    Field field_;
    std::map<ObjectId, std::shared_ptr<IObject>> objects_;
};

class IObject
{
public:
    virtual ~IObject() = default;

    /**
    * Move head to specified direction
    */
    virtual void frontMove(
        World::Field& field
        , Map::RelativeDirection frontDirection
    ) = 0;;

    /**
    * Move back to specified direction
    */
    virtual void backMove(
        World::Field& field
        , Map::RelativeDirection backDirection
    ) = 0;

    /**
    * Add one more point to the front
    */
    virtual void pushFrontGrain(
        World::Field& field
        , Map::RelativeDirection frontDirection
    ) = 0;

    virtual void popFrontGrain(
        World::Field& field
    ) = 0;

    /**
    * Add one more point to the back
    */
    virtual void pushBackGrain(
        World::Field& field
        , Map::RelativeDirection backDirection
    ) = 0;

    virtual void popBackGrain(
        World::Field& field
    ) = 0;

    virtual const World::Cell& lookTo(
        const World::Field& field
        , RelativeDirection to
        , size_t segment
    ) const = 0;

    virtual ObjectId id() const = 0;;
};

std::ostream& operator<<(std::ostream& is, const ObjectId& vect);
std::istream& operator>>(std::istream& is, ObjectId& vect);
std::ostream& operator<<(std::ostream& os, const Map::WorldCell& cell);
std::istream& operator>>(std::istream& is, Map::WorldCell& cell);


}  // namespace Map

