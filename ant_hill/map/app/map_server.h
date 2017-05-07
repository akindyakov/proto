#include <map/lib/world.h>
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
        std::unique_ptr<jsonrpc::AbstractServerConnector> connector
        , std::istream& fieldStream
    );

    Json::Value appear() override;
    int front_move(int direction, int id) override;
    int back_move(int direction, int id) override;
    int pick_ut_front(int direction, int id) override;
    int drop_front(int id) override;
    Json::Value look_to(int direction, int id, int segment) override;
    Json::Value get_pose(int id) override;
    int ping(int id) override;

public:
    void print(std::ostream& os) const;

private:
    Map::World world;
    std::mutex FieldMutex;
};
