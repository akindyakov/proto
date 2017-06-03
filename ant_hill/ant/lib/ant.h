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
    };
    LookInfo lookTo(
        Map::RelativeDirection direction
        , size_t segment = 0
    );

    Map::RelativeDirectionCurve getPose();

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
    bool locked;
};

std::ostream& operator<<(std::ostream& out, const DiscoveredCell& cell);

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

    Map::RelativeDirectionCurve
    findMaterial(
        Map::EMaterial what
    ) const;

    Map::Point
    findFreeSpace(
        const Map::Point& where
        , Map::Measure maxDist
    ) const;

    void printMap(std::ostream& out);

public:
    using FieldType = Map::Field<DiscoveredCell>;
    using SnakeType = Map::SnakeObj<FieldType>;

private:
    LocationClient client_;
    SnakeType snake_;
    FieldType grid_;
    Map::Chain<Map::RelativeDirection, Map::EMaterial> chain_;

public:
    static const DiscoveredCell UnknownCell;
};

class Scout
{
public:
    Scout(
        Map::JsonRPCClient& client
    );

    ~Scout() = default;

    Scout(const Scout&) = delete;
    Scout(Scout&&) = default;
    Scout& operator=(const Scout&) = delete;
    Scout& operator=(Scout&&) = default;

    bool run();

    void printMap(std::ostream& out) {
        location.printMap(out);
    }

private:
    void findTheWall();
    void moveAlongTheWall();
    bool followTheWay(
        const Map::RelativeDirectionCurve& way
    );
    bool discoverSomeSpace();

    Map::RelativeDirectionCurve
    findFreeSpace(
        const Map::Point& where
        , Map::Measure maxDist
    );

private:
    Location location;
};

}  // namespace Ant
