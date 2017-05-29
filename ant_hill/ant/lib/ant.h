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
};

std::ostream& operator<<(std::ostream& out, const DiscoveredCell& cell);

class Location
{
public:
    static constexpr auto sectionSideSize = Map::Measure{27};

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

    Map::EMaterial lookTo(
        Map::RelativeDirection direction
        , size_t segment = 0
    );

    const DiscoveredCell& mind(
        const Map::Point& pt
    ) const;

    const Map::Point& whereAmI() const;

    void printMap(std::ostream& out);

public:
    using FieldType = Map::Field<DiscoveredCell>;
    using SnakeType = Map::SnakeObj<FieldType>;

private:
    LocationClient client_;
    SnakeType snake_;
    FieldType grid_;
    Map::Chain<Map::RelativeDirection, Map::EMaterial> chain_;
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

private:
    Location location;
};

}  // namespace Ant
