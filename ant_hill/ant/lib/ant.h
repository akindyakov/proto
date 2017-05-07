#pragma once

#include <map/lib/chain.h>
#include <map/lib/world.h>

#include <map/rpc/client.h>


class IAnt
{
public:
    virtual void appear() = 0;
    virtual void step() = 0;
};

class Scout
    : public IAnt
{
public:
    Scout(
        Map::JsonRPC::Client& client
    );

    void appear() override;

    void step() override;

private:
    Map::JsonRPC::Client& client_;
    Map::ObjectId id_;
    Map::Chain<Map::RelativeDirection, Map::EMaterial> chain_;
};
