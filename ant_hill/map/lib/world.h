#pragma once

#include "2d_field.h"
#include "direction.h"
#include "map_symbols.h"
#include "snake.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>


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

const auto ObjectHash = [](const ObjectId& id) {
    return std::hash<ObjectId::Type>{}(id.id);
};

class World
{
public:
    using CellType = WorldCell;
    using FieldType = Field<CellType>;
    using SnakeType = SnakeObj<FieldType>;

    enum class Side {
        Front,
        Back,
    };

private:
    FieldType field_;
    std::mutex globalMutex;
    std::unordered_map<
        ObjectId,
        std::shared_ptr<SnakeType>,
        decltype(ObjectHash)
    > objects_;
    std::atomic<ObjectId::Type> nextFreeId_;

public:
    explicit World(
        std::istream& fieldStream
    );

    ObjectId appear();

    void move(
        ObjectId id
        , RelativeDirection direction
        , Side side
    );

    void pickUpGrain(
        ObjectId id
        , RelativeDirection direction
    );

    void dropGrain(
        ObjectId id
    );

    const CellType& lookTo(
        ObjectId id
        , RelativeDirection to
        , size_t segment
    ) const;

    std::vector<RelativeDirection> getPose(
        ObjectId id
    ) const;

    void ping(
        ObjectId id
    ) const;

    void print(std::ostream&) const;

private:
    std::shared_ptr<SnakeType> findObject(
        ObjectId id
    ) const;
};

std::ostream& operator<<(std::ostream& os, const ObjectId& vect);
std::istream& operator>>(std::istream& is, ObjectId& vect);

std::ostream& operator<<(std::ostream& os, const WorldCell& cell);
std::istream& operator>>(std::istream& is, WorldCell& cell);

World::SnakeType createSnakeOnField(
    World::FieldType& where
    , const std::vector<EMaterial>& body
    , ObjectId id
);

}  // namespace Map

