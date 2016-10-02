#include "map_server.h"

#include <map/lib/2d_field.h>

#include <map/rpc/server.h>

#include <tools/tests/ut.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

#include <iostream>

void App() {
    std::string text = R"FieldMap(10
10
iiiiiiiiii
is......wi
i.s....w.i
i..s..w..i
i...sw...i
i...ws...i
i..w.ms..i
i.w.m..s.i
iw...m..si
iiiiiiiiii
)FieldMap";
    auto in = std::istringstream(text);
    auto field = NField::ScanFromText(in);
    auto out = std::ostringstream();
    NField::PrintToText(out, field);
    auto outText = out.str();
    if (text != outText) {
        throw NAntHill::TException()
            << "Expected: " << text
            << "\nGot: " << outText;
    }
}

TMapServer::TMapServer(
    std::unique_ptr<jsonrpc::AbstractServerConnector>&& connector
    , NField::TField&& field
)
    : TConnectionOwner(std::move(connector))
    , NMap::NJsonRPC::TServer(TConnectionOwner::GetConnector())
    , Field(std::move(field))
{
}

int TMapServer::SeeGrain(int x, int y) {
    if (x < 0 || y < 0) {
        throw NAntHill::TException()
            << "Point must has positive coordinates "
            << "[" << x << ", " << y << "]";
    }
    Field.At({
        static_cast<NField::TMeasure>(x),
        static_cast<NField::TMeasure>(y)
    }).Grain.SeeMaterial();
    return x + y;
}
int TMapServer::MoveGroup(const Json::Value& grains) {
    std::cerr << "MoveGroup(...)" << std::endl;
    // TODO:
    return 0;
}

Json::Value TMapServer::YieldMe(
    const Json::Value& grains
    , const Json::Value& place
) {
    // TODO:
    auto point = Json::Value{};
    point["x"] = 1;
    point["y"] = 1;
    return point;
}

int TMapServer::Ping() {
    std::cerr << "Ping()" << std::endl;
    return 0;
}
