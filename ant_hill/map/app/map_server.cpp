#include "map_server.h"

#include <map/lib/2d_field.h>
#include <map/lib/transaction.h>

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
    auto field = Field::ScanFromText(in);
    auto out = std::ostringstream();
    Field::PrintToText(out, field);
    auto outText = out.str();
    if (text != outText) {
        throw AntHill::Exception()
            << "Expected: " << text
            << "\nGot: " << outText;
    }
}

MapServer::MapServer(
    std::unique_ptr<jsonrpc::AbstractServerConnector>&& connector
    , Field::Field& field
)
    : ConnectionHolder(std::move(connector))
    , Map::JsonRPC::Server(ConnectionHolder::GetConnector())
    , Field(field)
{
}

int MapServer::SeeGrain(int x, int y) {
    if (x < 0 || y < 0) {
        throw AntHill::Exception()
            << "Point must has positive coordinates "
            << "[" << x << ", " << y << "]";
    }

    try {
        Field.At({
            static_cast<Field::Measure>(x),
            static_cast<Field::Measure>(y)
        }).Grain.SeeMaterial();
    }
    catch(std::out_of_range& err) {
        Json::Value data;
        data.append(33);
        throw jsonrpc::JsonRpcException(-32099, "User exception", data);
    }
    return x + y;
}

int MapServer::MoveGroup(const Json::Value& params) {
    std::cerr << "MapServer::MoveGroup\n";
    auto tr = Field::MoveTransaction{};
    for (const auto& grain : params["grains"]) {
        const Json::Value& from = grain["from"];
        auto x = static_cast<Field::Measure>(from["x"].asInt());
        auto y = static_cast<Field::Measure>(from["y"].asInt());
        tr.Add(
            {x, y},
            static_cast<Field::Direction::ECompass>(grain["direction"].asInt())
        );
        std::cerr << "MapServer::MoveGroup -- Point: (" << x << "; " << y << ")\n";
    }
    std::lock_guard<std::mutex> lock{FieldMutex};
    if (tr.Apply(Field)) {
        return 0;
    }
    return 1;
}

Json::Value MapServer::YieldMe(
    const Json::Value& params
    , const Json::Value& place
) {
    std::cerr << "MapServer::YieldMe\n";
    auto tr = Field::AppearanceTransaction{};
    for (const auto& grain : params["grains"]) {
        const Json::Value& point = grain["point"];
        auto x = static_cast<Field::Measure>(point["x"].asInt());
        auto y = static_cast<Field::Measure>(point["y"].asInt());
        tr.Add(
            {x, y},
            static_cast<EMaterial>(grain["material"].asInt())
        );
        std::cerr << "MapServer::YieldMe -- Point: (" << x << "; " << y << ")\n";
    }
    auto shift = this->YieldMeImpl(tr);
    auto jsonShift = Json::Value{};
    jsonShift["x"] = shift.X;
    jsonShift["y"] = shift.Y;
    return jsonShift;
}

Field::Vector MapServer::YieldMeImpl(Field::AppearanceTransaction& tr) {
    std::lock_guard<std::mutex> lock{FieldMutex};
    return tr.Apply(Field);
}

int MapServer::Ping() {
    std::cerr << "MapServer::Ping\n";
    return 0;
}
