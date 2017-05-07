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
        for (const auto& dir : {
            Map::RelativeDirection::Forward(),
            Map::RelativeDirection::Left(),
            Map::RelativeDirection::Backward(),
            Map::RelativeDirection::Right(),
        }) {
            const auto segment = 0;  // head
            auto view = this->client_.look_to(
                dir.toInt(),
                id_.id,
                segment
            );
            if (view["material"].asInt() == 0) {
                this->client_.front_move(
                    dir.toInt(),
                    id_.id
                );
                break;
            }
        }
    } catch (const jsonrpc::JsonRpcException& err) {
        std::cerr << err.what();
    }
}
