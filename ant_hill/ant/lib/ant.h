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

    struct LookInfo {
        Map::EMaterial material = Map::EMaterial::Unknown;
        Map::ObjectId ownerId = Map::ObjectId::Invalid();
    };
    LookInfo lookTo(
        Map::RelativeDirection direction
        , size_t segment
    ) {
        auto ret = LookInfo{};
        try {
            auto jRet = this->client_.look_to(
                direction.toInt(),
                this->id_.id,
                segment
            );
            ret.ownerId = Map::ObjectId(
                jRet["owner_id"].asInt()
            );
            ret.material = static_cast<Map::EMaterial>(
                jRet["material"].asInt()
            );
        } catch (const jsonrpc::JsonRpcException&) {
            // TODO: log warnign here
            std::cerr << "warning: empty [look_to] responce";
        }
        return ret;
    }

    Map::RelativeDirectionCurve getPose() {
        auto pose = Map::RelativeDirectionCurve{};
        try {
            auto jPose = this->client_.get_pose(
                this->id_.id
            );
            for (const auto& p : jPose["pose"]) {
                pose.push_back(
                    Map::RelativeDirection::fromInt(
                        p.asInt()
                    )
                );
            }
        } catch (const jsonrpc::JsonRpcException) {
            // TODO: log warnign here
            std::cerr << "warning: empty [get_pose] responce";
        }
        return pose;
    }

private:
    Map::JsonRPCClient& client_;
    Map::ObjectId id_;
};

class DiscoveredCell
{
public:
    DiscoveredCell() = default;

    bool isFree() const {
        return false;
    }

public:
    Map::EMaterial material = Map::EMaterial::Unknown;
};

class Location
{
public:
    static constexpr auto sectionSideSize = Map::Measure{27};

    explicit Location(
        LocationClient client
    )
        : client_(std::move(client))
        , snake_(
            Map::Point(0, 0),
            Map::RelativeCurveToCurve(
                client_.getPose()
            )
        )
        , grid_(
            Map::Vector(sectionSideSize, sectionSideSize),
            Map::Point(sectionSideSize/-2, sectionSideSize/-2)
        )
    {
    }

    bool frontMove(
        Map::RelativeDirection direction
    ) {
        return false;
    }

    bool backMove(
        Map::RelativeDirection direction
    ) {
        return false;
    }

    bool pickUpFront(
        Map::RelativeDirection direction
    ) {
        return false;
    }

    bool dropFront() {
        return false;
    }

    bool lookTo(
        Map::RelativeDirection direction
        , size_t segment
    ) {
        return false;
    }

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

private:
    void findTheWall();
    void moveAlongTheWall();

private:
    Location location;
};

}  // namespace Ant
