#include "map_server.h"

#include <map/lib/2d_field.h>
#include <map/lib/direction.h>

#include <map/rpc/server.h>

#include <lib/http_error.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

#include <iostream>


MapServer::MapServer(
    std::unique_ptr<jsonrpc::AbstractServerConnector> connector
    , std::istream& fieldStream
)
    : ConnectionHolder(std::move(connector))
    , Map::JsonRPCServer(ConnectionHolder::GetConnector())
    , world(fieldStream)
{
}


Json::Value MapServer::appear() {
    auto result = Json::Value(Json::objectValue);
    try {
        result["id"] = this->world.appear().id;
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return result;
}

Json::Value MapServer::front_move(int direction, int id) {
    auto result = Json::Value(Json::objectValue);
    try {
        this->world.move(
            Map::ObjectId(id),
            Map::RelativeDirection(direction),
            Map::World::Side::Front
        );
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return result;
}

Json::Value MapServer::back_move(int direction, int id) {
    auto result = Json::Value(Json::objectValue);
    try {
        this->world.move(
            Map::ObjectId(id),
            Map::RelativeDirection(direction),
            Map::World::Side::Back
        );
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return result;
}

Json::Value MapServer::pick_up_front(int direction, int id) {
    auto result = Json::Value(Json::objectValue);
    try {
        this->world.pickUpGrain(
            Map::ObjectId(id),
            Map::RelativeDirection(direction)
        );
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return result;
}

Json::Value MapServer::drop_front(int id) {
    auto result = Json::Value(Json::objectValue);
    try {
        this->world.dropGrain(
            Map::ObjectId(id)
        );
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return result;
}

Json::Value MapServer::look_to(int direction, int id, int segment) {
    auto val = Json::Value{};
    try {
        auto cell = this->world.lookTo(
            Map::ObjectId(id),
            Map::RelativeDirection(direction),
            static_cast<size_t>(segment)
        );
        val["owner_id"] = cell.objectId.id;
        val["material"] = static_cast<int>(cell.grain);
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return val;
}

Json::Value MapServer::get_pose(int id) {
    auto val = Json::Value{};
    try {
        auto pose = this->world.getPose(
            Map::ObjectId(id)
        );
        auto& p = val["pose"];
        p = Json::arrayValue;
        for (const auto& dir : pose) {
            p.append(dir.toInt());
        }
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return val;
}

Json::Value MapServer::ping(int id) {
    auto result = Json::Value(Json::objectValue);
    try {
        this->world.ping(
            Map::ObjectId(id)
        );
    } catch (const Lib::HTTP::Error& err) {
        throw jsonrpc::JsonRpcException(
            err.code(), err.message()
        );
    }
    return result;
}

void MapServer::print(std::ostream& os) const {
    this->world.print(os);
}
