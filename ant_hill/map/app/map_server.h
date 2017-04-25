#include <map/lib/2d_field.h>
#include <map/lib/transaction.h>
#include <map/rpc/server.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

class ConnectionHolder {
public:
    ConnectionHolder(
        std::unique_ptr<jsonrpc::AbstractServerConnector>&& connector
    )
        : Connector(std::move(connector))
    {
    }
    ConnectionHolder(const ConnectionHolder&) = delete;
    ConnectionHolder(ConnectionHolder&&) = default;
    ConnectionHolder& operator=(const ConnectionHolder&) = delete;
    ConnectionHolder& operator=(ConnectionHolder&&) = default;

    jsonrpc::AbstractServerConnector& GetConnector() {
        return *Connector;
    }

private:
    std::unique_ptr<jsonrpc::AbstractServerConnector> Connector;
};

class MapServer
    : public ConnectionHolder
    , public Map::JsonRPC::Server
{
public:
    MapServer(
        std::unique_ptr<jsonrpc::AbstractServerConnector>&& connector
        , Field::Field& field
    );

    int SeeGrain(int x, int y) override;
    int MoveGroup(const Json::Value& grains) override;

    Json::Value YieldMe(
        const Json::Value& body
    ) override;

    int Ping() override;

private:
    Field::Point YieldMeImpl(Field::AppearanceTransaction& tr);

private:
    Field::Field& Field;
    std::mutex FieldMutex;
};
