#include <map/rpc/server.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

class TMapServer
    : public NMap::NJsonRPC::TServer
{
public:
    TMapServer(jsonrpc::AbstractServerConnector &conn);

    int SeeGrain(int x, int y) override;
    int MoveGroup(const Json::Value& grains) override;

    Json::Value YieldMe(
        const Json::Value& grains
        , const Json::Value& place
    ) override;

    int Ping() override;
};
