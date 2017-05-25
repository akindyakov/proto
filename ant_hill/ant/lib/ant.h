#pragma once

#include <map/lib/chain.h>
#include <map/lib/world.h>

#include <map/rpc/client.h>


class AntState
{
public:
    explicit KnownArea(
        Map::JsonRPCClient& client
    ) {
    }

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

    bool lookTo(
        Map::RelativeDirection direction
        , int segment
    ) const;

private:
    class KnownAreaCell
    {
    public:
        Map::EMaterial material;
    };

    Map::Field<KnownAreaCell> grid;
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

    void appear();
    bool run();

private:
    void findTheWall();
    void moveAlongTheWall();

private:
    Map::JsonRPCClient& client_;
    Map::ObjectId id_;
    Map::Chain<Map::RelativeDirection, Map::EMaterial> chain_;
};
