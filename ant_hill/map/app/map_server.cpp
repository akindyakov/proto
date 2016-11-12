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
    : TConnectionHolder(std::move(connector))
    , NMap::NJsonRPC::TServer(TConnectionHolder::GetConnector())
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

int TMapServer::MoveGroup(const Json::Value& params) {
    std::cerr << "MoveGroup(...)" << std::endl;
    auto tr = NField::TMoveTransaction{};
    for (const auto& grain : params["grains"]) {
        const Json::Value& from = grain["from"];
        tr.Add({
                static_cast<NField::TMeasure>(from["x"].asInt()),
                static_cast<NField::TMeasure>(from["y"].asInt())
            },
            static_cast<NField::ECompass>(grain["direction"].asInt())
        );
    }
    std::lock_guard<std::mutex> lock{FieldMutex};
    if (tr.Apply(Field)) {
        return 0;
    }
    return 1;
}

Json::Value TMapServer::YieldMe(
    const Json::Value& params
    , const Json::Value& place
) {
    auto tr = NField::TAppearanceTransaction{};
    for (const auto& grain : params["grains"]) {
        const Json::Value& point = grain["point"];
        tr.Add(
            {
                static_cast<NField::TMeasure>(point["x"].asInt()),
                static_cast<NField::TMeasure>(point["y"].asInt())
            },
            static_cast<EMaterial>(grain["material"].asInt())
        );
    }
    auto shift = this->YieldMeImpl(tr);
    auto jsonShift = Json::Value{};
    jsonShift["x"] = shift.X;
    jsonShift["y"] = shift.Y;
    return jsonShift;
}

NField::TVector TMapServer::YieldMeImpl(NField::TAppearanceTransaction& tr) {
    std::lock_guard<std::mutex> lock{FieldMutex};
    return tr.Apply(Field);
}

int TMapServer::Ping() {
    std::cerr << "Ping()" << std::endl;
    return 0;
}
