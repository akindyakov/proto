#pragma once

#include <map/lib/chain.h>
#include <map/lib/world.h>
#include <map/lib/snake.h>

#include <map/rpc/client.h>

namespace Ant {

class LocationClient
{
public:
    LocationClient(const LocationClient&) = delete;
    LocationClient(LocationClient&&) = default;
    LocationClient& operator=(const LocationClient&) = delete;
    LocationClient& operator=(LocationClient&&) = default;

    explicit LocationClient(
        Map::JsonRPCClient& client
    );

    bool frontMove(
        Map::RelativeDirection direction
    );

    bool backMove(
        Map::RelativeDirection direction
    );

    bool pickUpFront(
        Map::RelativeDirection direction
    );

    bool dropFront();

    struct LookInfo {
        Map::EMaterial material = Map::EMaterial::Unknown;
        Map::ObjectId ownerId = Map::ObjectId::Invalid();
        bool forbidden = false;
    };
    LookInfo lookTo(
        Map::RelativeDirection direction
        , size_t segment = 0
    );

    Map::RelativeDirectionCurve getPose();

    Map::ObjectId id() const noexcept {
        return this->id_;
    }

private:
    Map::JsonRPCClient& client_;
    Map::ObjectId id_;
};

class DiscoveredCell
{
public:
    DiscoveredCell() = default;

public:
    Map::EMaterial material = Map::EMaterial::Unknown;
    bool locked = false;
};

std::ostream& operator<<(std::ostream& out, const DiscoveredCell& cell);

using FieldInMemory = Map::Field<DiscoveredCell>;

class Location
{
public:
    static constexpr auto sectionSideSize = Map::Measure{51};

    explicit Location(
        LocationClient client
    );

    bool frontMove(
        Map::RelativeDirection direction
    );

    bool backMove(
        Map::RelativeDirection direction
    );

    bool pickUpFront(
        Map::RelativeDirection direction
    );

    bool dropFront();

    const DiscoveredCell& lookTo(
        Map::RelativeDirection direction
        , size_t segment = 0
    );

    const DiscoveredCell& mind(
        const Map::Point& pt
    ) const;

    const Map::Point& whereAmI() const;
    Map::Point whereIsMyTail() const;

    Map::RelativeDirectionCurve
    findMaterial(
        const Map::Point& where
        , Map::EMaterial what
    ) const;

    Map::Point
    findFreeSpace(
        const Map::Point& where
        , Map::Measure maxDist
    ) const;

    void printMap(std::ostream& out);

    Map::ObjectId id() const noexcept {
        return this->client_.id();
    }

public:
    using SnakeType = Map::SnakeObj<FieldInMemory>;

private:
    LocationClient client_;
    SnakeType snake_;
    FieldInMemory grid_;

public:
    static const DiscoveredCell UnknownCell;
};

bool discoverSomeSpace(
    Location& location
);

bool followTheWay(
    Location& location
    , const Map::RelativeDirectionCurve& way
);

bool followTheWayBack(
    Location& location
    , const Map::RelativeDirectionCurve& way
);

}  // namespace Ant
