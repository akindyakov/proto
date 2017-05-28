#include "ant.h"

#include <map/lib/map_symbols.h>

#include <tools/exception.h>


namespace Ant {

std::ostream& operator<<(std::ostream& out, const DiscoveredCell& cell) {
    out << Map::GetSymbolMap().GetSymbol(cell.material);
    return out;
}

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
    try {
        for (const auto& dir : {
            Map::RelativeDirection::Forward(),
            Map::RelativeDirection::Left(),
            Map::RelativeDirection::Right(),
            Map::RelativeDirection::Backward(),
        }) {
            this->location.lookTo(dir, 0);
        }
        for (const auto& dir : {
            Map::RelativeDirection::Forward(),
            Map::RelativeDirection::Left(),
            Map::RelativeDirection::Right(),
            Map::RelativeDirection::Backward(),
        }) {
            if (this->location.frontMove(dir)) {
                return true;
            }
        }
    } catch (const jsonrpc::JsonRpcException& err) {
        std::cerr << err.what();
    }
    return false;
}

}  // namespace Ant
