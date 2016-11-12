#include <map/lib/2d_field.h>
#include <map/lib/transaction.h>
#include <map/rpc/server.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

class TConnectionHolder {
public:
    TConnectionHolder(
        std::unique_ptr<jsonrpc::AbstractServerConnector>&& connector
    )
        : Connector(std::move(connector))
    {
    }
    TConnectionHolder(const TConnectionHolder&) = delete;
    TConnectionHolder(TConnectionHolder&&) = default;
    TConnectionHolder& operator=(const TConnectionHolder&) = delete;
    TConnectionHolder& operator=(TConnectionHolder&&) = default;

    jsonrpc::AbstractServerConnector& GetConnector() {
        return *Connector;
    }

private:
    std::unique_ptr<jsonrpc::AbstractServerConnector> Connector;
};

class TMapServer
    : public TConnectionHolder
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
    NField::TVector YieldMeImpl(NField::TAppearanceTransaction& tr);

private:
    NField::TField Field;
    std::mutex FieldMutex;
};
