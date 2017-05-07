#include "map_server.h"

#include <map/lib/2d_field.h>
#include <map/lib/transaction.h>

#include <map/rpc/server.h>

#include <tools/tests/ut.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

#include <iostream>


MapServer::MapServer(
    std::unique_ptr<jsonrpc::AbstractServerConnector> connector
    , std::istream& fieldStream
)
    : ConnectionHolder(std::move(connector))
    , Map::JsonRPC::Server(ConnectionHolder::GetConnector())
    , world(fieldStream)
{
}


Json::Value MapServer::appear() {
    auto val = Json::Value{};
    val["id"] = this->world.appear().id;
    return val;
}

int MapServer::front_move(int direction, int id) {
    this->world.move(
        Map::ObjectId(id),
        Map::RelativeDirection(direction),
        Map::World::Side::Front
    );
    return 0;
}

int MapServer::back_move(int direction, int id) {
    this->world.move(
        Map::ObjectId(id),
        Map::RelativeDirection(direction),
        Map::World::Side::Back
    );
    return 0;
}

int MapServer::pick_ut_front(int direction, int id) {
    this->world.pickUpGrain(
        Map::ObjectId(id),
        Map::RelativeDirection(direction),
        Map::World::Side::Front
    );
    return 0;
}

int MapServer::drop_front(int id) {
    this->world.dropGrain(
        Map::ObjectId(id),
        Map::World::Side::Front
    );
    return 0;
}

Json::Value MapServer::look_to(int direction, int id, int segment) {
    auto cell = this->world.lookTo(
        Map::ObjectId(id),
        Map::RelativeDirection(direction),
        static_cast<size_t>(segment)
    );
    auto val = Json::Value{};
    val["owner_id"] = cell.objectId.id;
    val["material"] = static_cast<int>(cell.grain);
    return val;
}

Json::Value MapServer::get_pose(int id) {
    auto pose = this->world.getPose(
        Map::ObjectId(id)
    );
    auto val = Json::Value{};
    auto& p = val["pose"];
    p = Json::arrayValue;
    for (const auto& dir : pose) {
        p.append(dir.toInt());
    }
    return val;
}

int MapServer::ping(int id) {
    this->world.ping(
        Map::ObjectId(id)
    );
    return 0;
}

void MapServer::print(std::ostream& os) const {
    this->world.print(os);
}
