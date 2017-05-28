#include "ant.h"

#include <tools/exception.h>


namespace Ant {

Scout::Scout(
    Map::JsonRPCClient& client
)
    : location(
        LocationClient(client)
    )
{
}


void Scout::findTheWall() {
}

void Scout::moveAlongTheWall() {
}

bool Scout::run() {
//   try {
//       for (const auto& dir : {
//           Map::RelativeDirection::Forward(),
//           Map::RelativeDirection::Left(),
//           Map::RelativeDirection::Backward(),
//           Map::RelativeDirection::Right(),
//       }) {
//           const auto segment = 0;  // head
//           auto view = this->client_.look_to(
//               dir.toInt(),
//               id_.id,
//               segment
//           );
//           if (view["material"].asInt() == 0) {
//               this->client_.front_move(
//                   dir.toInt(),
//                   id_.id
//               );
//               break;
//           }
//       }
//   } catch (const jsonrpc::JsonRpcException& err) {
//       std::cerr << err.what();
//   }
    return false;
}

}  // namespace Ant
