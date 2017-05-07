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


int MapServer::appear() {
    return 0;
}

int MapServer::front_move(int direction, int id) {
    return 0;
}

int MapServer::back_move(int direction, int id) {
    return 0;
}

int MapServer::pick_ut_front(int direction, int id) {
    return 0;
}

int MapServer::drop_front(int direction, int id) {
    return 0;
}

int MapServer::look_to(int direction, int id) {
    return 0;
}

Json::Value MapServer::get_pose(int id) {
    return Json::Value{};
}

int MapServer::ping(int id) {
    std::cerr << "MapServer::Ping\n";
    return 0;
}

void MapServer::print(std::ostream& os) const {
    this->world.print(os);
}
