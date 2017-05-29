#include "ant.h"

#include <map/lib/map_symbols.h>
#include <map/lib/spiral.h>

#include <tools/exception.h>


namespace Ant {

LocationClient::LocationClient(
    Map::JsonRPCClient& client
)
    : client_(client)
    , id_(Map::ObjectId::Invalid())
{
    auto ret = client_.appear();
    try {
        this->id_ = Map::ObjectId(
            ret["id"].asInt()
        );
    } catch (const jsonrpc::JsonRpcException) {
    }
    if (!this->id_.isValid()) {
        throw Exception("Invalid id, wtf?");
    }
}

bool LocationClient::frontMove(
    Map::RelativeDirection direction
) {
    try {
        this->client_.front_move(
            direction.toInt(),
            this->id_.id
        );
    } catch (const jsonrpc::JsonRpcException) {
        return false;
    }
    return true;
}

bool LocationClient::backMove(
    Map::RelativeDirection direction
) {
    try {
        this->client_.back_move(
            direction.toInt(),
            this->id_.id
        );
    } catch (const jsonrpc::JsonRpcException) {
        return false;
    }
    return true;
}

bool LocationClient::pickUpFront(
    Map::RelativeDirection direction
) {
    try {
        this->client_.pick_up_front(
            direction.toInt(),
            this->id_.id
        );
    } catch (const jsonrpc::JsonRpcException) {
        return false;
    }
    return true;
}

bool LocationClient::dropFront() {
    try {
        this->client_.drop_front(
            this->id_.id
        );
    } catch (const jsonrpc::JsonRpcException&) {
        return false;
    }
    return true;
}

LocationClient::LookInfo LocationClient::lookTo(
    Map::RelativeDirection direction
    , size_t segment
) {
    auto ret = LookInfo{};
    try {
        auto jRet = this->client_.look_to(
            direction.toInt(),
            this->id_.id,
            segment
        );
        ret.ownerId = Map::ObjectId(
            jRet["owner_id"].asInt()
        );
        ret.material = static_cast<Map::EMaterial>(
            jRet["material"].asInt()
        );
    } catch (const jsonrpc::JsonRpcException&) {
        // TODO: log warnign here
        std::cerr << "warning: empty [look_to] responce";
    }
    return ret;
}

Map::RelativeDirectionCurve LocationClient::getPose() {
    auto pose = Map::RelativeDirectionCurve{};
    try {
        auto jPose = this->client_.get_pose(
            this->id_.id
        );
        for (const auto& p : jPose["pose"]) {
            pose.push_back(
                Map::RelativeDirection::fromInt(
                    p.asInt()
                )
            );
        }
    } catch (const jsonrpc::JsonRpcException) {
        // TODO: log warnign here
        std::cerr << "warning: empty [get_pose] responce";
    }
    return pose;
}

std::ostream& operator<<(std::ostream& out, const DiscoveredCell& cell) {
    out << Map::GetSymbolMap().GetSymbol(cell.material);
    return out;
}

Location::Location(
    LocationClient client
)
    : client_(std::move(client))
    , snake_(
        Map::Point(0, 0),
        Map::RelativeCurveToCurve(
            client_.getPose()
        )
    )
    , grid_(
        Map::Vector(sectionSideSize, sectionSideSize),
        Map::Point(sectionSideSize/-2, sectionSideSize/-2)
    )
{
    auto body = snake_.getBody();
    this->grid_.at(body[0]).material = Map::EMaterial::AntHead;
    this->grid_.at(body[1]).material = Map::EMaterial::AntBody;
}

bool Location::frontMove(
    Map::RelativeDirection direction
) {
    auto success = this->client_.frontMove(direction);
    if (success) {
        this->snake_.frontMove(this->grid_, direction);
    } else {
        this->lookTo(direction);
    }
    return success;
}

bool Location::backMove(
    Map::RelativeDirection direction
) {
    auto success = this->client_.backMove(direction);
    if (success) {
        this->snake_.backMove(this->grid_, direction);
    } else {
        this->lookTo(direction, this->snake_.size());
    }
    return success;
}

bool Location::pickUpFront(
    Map::RelativeDirection direction
) {
    return false;
}

bool Location::dropFront() {
    return false;
}

const DiscoveredCell& Location::lookTo(
    Map::RelativeDirection direction
    , size_t segment
) {
    auto pt = this->snake_.lookTo(direction, segment);
    auto& cell = this->grid_.at(pt);

    auto resp = this->client_.lookTo(direction, segment);
    if (resp.material != Map::EMaterial::Unknown) {
        if (resp.ownerId.isValid()) {
            // this is someone else
            // TODO: do smth to connect
        }
        cell.material = resp.material;
    }
    return cell;
}

const DiscoveredCell Location::UnknownCell = DiscoveredCell{};

const DiscoveredCell& Location::mind(
    const Map::Point& pt
) const {
    if (this->grid_.inRange(pt)) {
        return this->grid_.at(pt);
    }
    return UnknownCell;
}

const Map::Point& Location::whereAmI() const {
    return snake_.head();
}

void Location::printMap(std::ostream& out) {
    Map::PrintToText(out, this->grid_);
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

Map::Point
Location::findMaterial(
    Map::EMaterial what
    , const Map::Point& where
    , Map::Measure maxDist
) const {
    auto spiral = Map::Spiral(where);
    while (spiral.radius() != maxDist) {
        auto pt = spiral.next();
        if (
            this->grid_.inRange(pt)
            && what == this->grid_.at(pt).material
        ) {
            return pt;
        }
    }
    return where;
}

bool Scout::run() {
    try {
        for (const auto& dir : {
            Map::RelativeDirection::Forward(),
            Map::RelativeDirection::Left(),
            Map::RelativeDirection::Right(),
        }) {
            this->location.lookTo(dir);
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
