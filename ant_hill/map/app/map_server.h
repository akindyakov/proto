#include <map/lib/2d_field.h>
#include <map/rpc/server.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

class TConnectionOwner {
public:
    TConnectionOwner(
        std::unique_ptr<jsonrpc::AbstractServerConnector>&& connector
    )
        : Connector(std::move(connector))
    {
    }
    TConnectionOwner(const TConnectionOwner&) = delete;
    TConnectionOwner(TConnectionOwner&&) = default;
    TConnectionOwner& operator=(const TConnectionOwner&) = delete;
    TConnectionOwner& operator=(TConnectionOwner&&) = default;

    jsonrpc::AbstractServerConnector& GetConnector() {
        return *Connector;
    }

private:
    std::unique_ptr<jsonrpc::AbstractServerConnector> Connector;
};

class TMapServer
    : public TConnectionOwner
    , public NMap::NJsonRPC::TServer
{
public:
    TMapServer(
        std::unique_ptr<jsonrpc::AbstractServerConnector>&& connector
        , NField::TField&& field
    );

    int SeeGrain(int x, int y) override;
    int MoveGroup(const Json::Value& grains) override;

    Json::Value YieldMe(
        const Json::Value& grains
        , const Json::Value& place
    ) override;

    int Ping() override;

private:
    NField::TField Field;
};
