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
    )
        : client_(client)
        , id_(Map::ObjectId::Invalid())
    {
        auto ret = client_.appear();
        try {
            this->id_ = Map::ObjectId(
                ret["id"].asInt()
            );
        } catch (const jsonrpc::JsonRpcException) {
        }
        if (!this->id_.isValid()) {
            throw Exception("Invalid id, wtf?");
        }
    }

    bool frontMove(
        Map::RelativeDirection direction
    ) {
        try {
            this->client_.front_move(
                direction.toInt(),
                this->id_.id
            );
        } catch (const jsonrpc::JsonRpcException) {
            return false;
        }
        return true;
    }

    bool backMove(
        Map::RelativeDirection direction
    ) {
        try {
            this->client_.back_move(
                direction.toInt(),
                this->id_.id
            );
        } catch (const jsonrpc::JsonRpcException) {
            return false;
        }
        return true;
    }

    bool pickUpFront(
        Map::RelativeDirection direction
    ) {
        try {
            this->client_.pick_up_front(
                direction.toInt(),
                this->id_.id
            );
        } catch (const jsonrpc::JsonRpcException) {
            return false;
        }
        return true;
    }

    bool dropFront() {
        try {
            this->client_.drop_front(
                this->id_.id
            );
        } catch (const jsonrpc::JsonRpcException&) {
            return false;
        }
        return true;
    }

    bool lookTo(
        Map::RelativeDirection direction
        , size_t segment
    ) {
        try {
            this->client_.look_to(
                direction.toInt(),
                this->id_.id,
                segment
            );
        } catch (const jsonrpc::JsonRpcException) {
            return false;
        }
        return true;
    }

    bool getPose() {
        try {
            this->client_.get_pose(
                this->id_.id
            );
        } catch (const jsonrpc::JsonRpcException) {
            return false;
        }
        return true;
    }

private:
    Map::JsonRPCClient& client_;
    Map::ObjectId id_;
};

class Location
{
public:
    explicit Location(
        LocationClient client
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
        , size_t segment
    ) const;

public:
    class DiscoveredCell
    {
    public:
        explicit DiscoveredCell() = default;
        Map::EMaterial material = Map::EMaterial::Unknown;
    };

    using FieldType = Map::Field<DiscoveredCell>;
    using SnakeType = Map::SnakeObj<FieldType>;

private:
    SnakeType snake_;
    FieldType grid_;
    Map::Chain<Map::RelativeDirection, Map::EMaterial> chain_;
    LocationClient client_;
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

private:
    void findTheWall();
    void moveAlongTheWall();

private:
    Location location;
};

}  // namespace Ant
