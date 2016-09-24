#include <map/rpc/server.h>

#include <jsonrpccpp/server/connectors/httpserver.h>

#include <iostream>

class TMapServer
    : public NMap::NJsonRPC::TServer
{
    public:
        TMapServer(jsonrpc::AbstractServerConnector &conn)
            : NMap::NJsonRPC::TServer(conn)
        {
        }

        int SeeGrain(int x, int y) override {
            std::cerr << "GetGrain(" << x << ", " << y << ")" << std::endl;
            // TODO:
            return x + y;
        }
        int MoveGroup(const Json::Value& grains) override {
            std::cerr << "MoveGroup(...)" << std::endl;
            // TODO:
            // TODO:
            return 0;
        }
        int Ping() override {
            std::cerr << "Ping()" << std::endl;
            // TODO:
            return 0;
        }
};


int main()
{
    // TODO: Put argument parsing here
    jsonrpc::HttpServer httpserver(8383);
    TMapServer s(httpserver);
    s.StartListening();
    std::cin.get();
    s.StopListening();
    return 0;
}
