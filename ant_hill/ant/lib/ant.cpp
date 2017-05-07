#include "ant.h"

#include <tools/exception.h>

Scout::Scout(
    Map::JsonRPC::Client& client
)
    : client_(client)
    , id_(Map::ObjectId::Invalid())
{
}


void Scout::appear() {
    auto ret = client_.appear();
    this->id_ = Map::ObjectId(
        ret["id"].asInt()
    );
    if (!this->id_.isValid()) {
        throw Exception("Invalid id, wtf?");
    }
}

void Scout::step() {
    try {
        auto view = this->client_.look_to(
            0,
            id_.id,
            0
        );
        if (view["material"] == 0) {
            this->client_.front_move(
                0,
                id_.id
            );
        }
    } catch (const jsonrpc::JsonRpcException& err) {
        std::cerr << err.what();
    }
}
