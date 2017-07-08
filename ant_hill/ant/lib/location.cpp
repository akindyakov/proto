#include "location.h"

#include <map/lib/find_a_way.h>
#include <map/lib/map_symbols.h>
#include <map/lib/spiral.h>

#include <lib/exception.h>

#include <chrono>
#include <thread>


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
        ret.forbidden = true;
        // TODO: log warnign here
        std::cerr << "warning: [look_to] error in responce\n";
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
        std::cerr << "warning: empty [get_pose] responce\n";
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
        Map::Vector(7, 7),
        Map::Point(-3, -3)
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
    }
    for (const auto& dir : {
        Map::RelativeDirection::Forward(),
        Map::RelativeDirection::Left(),
        Map::RelativeDirection::Right(),
    }) {
        this->lookTo(dir);
    }
    /*dbg*/ std::this_thread::sleep_for(std::chrono::milliseconds(100));
    /*dbg*/ printMap(std::cerr);
    return success;
}

bool Location::backMove(
    Map::RelativeDirection direction
) {
    auto success = this->client_.backMove(direction);
    if (success) {
        this->snake_.backMove(this->grid_, direction);
    }
    auto lastSegment = this->snake_.size() - 1;
    for (const auto& dir : {
        Map::RelativeDirection::Backward(),
        Map::RelativeDirection::Left(),
        Map::RelativeDirection::Right(),
    }) {
        this->lookTo(dir, lastSegment);
    }
    /*dbg*/ std::this_thread::sleep_for(std::chrono::milliseconds(100));
    /*dbg*/ printMap(std::cerr);
    return success;
}

bool Location::pickUpFront(
    Map::RelativeDirection direction
) {
    auto success = this->client_.pickUpFront(direction);
    if (success) {
        this->snake_.pushFrontGrain(direction);
    }
    return success;
}

bool Location::dropFront() {
    auto success = this->client_.dropFront();
    if (success) {
        this->snake_.popFrontGrain();
    }
    return success;
}

const DiscoveredCell& Location::lookTo(
    Map::RelativeDirection direction
    , size_t segment
) {
    auto pt = this->snake_.lookTo(direction, segment);
    if (!this->grid_.inRange(pt)) {
        this->grid_.extendFor(pt);
    }
    auto& cell = this->grid_.at(pt);
    auto resp = this->client_.lookTo(direction, segment);
    if (resp.forbidden) {
        // FIXME: use fullsize status bitset here
        cell.locked = true;
    } else {
        if (resp.ownerId.isValid()) {
            // this is someone else
            // TODO: do smth to connect
            cell.material = Map::EMaterial::EmptySpace;
        } else {
            cell.material = resp.material;
        }
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

Map::Point Location::whereIsMyTail() const {
    return snake_.tail();
}

void Location::printMap(std::ostream& out) {
    Map::PrintToText(out, this->grid_);
}

Map::Point
Location::findFreeSpace(
    const Map::Point& where
    , Map::Measure maxDist
) const {
    auto spiral = Map::Spiral(where);
    while (spiral.radius() != maxDist) {
        auto pt = spiral.next();
        if (
            this->grid_.inRange(pt)
            && this->grid_.at(pt).material == Map::EMaterial::EmptySpace
        ) {
            return pt;
        }
    }
    return where;
}

Map::RelativeDirectionCurve
Location::findMaterial(
    const Map::Point& where
    , Map::EMaterial what
) const {
    // FIXME
    auto cost = [what=what, &field=this->grid_](const Map::Point& pt) {
        return (
            field.at(pt).material == Map::EMaterial::EmptySpace
            || (field.at(pt).material == what && !field.at(pt).locked)
            ? 1
            : -1
        );
    };
    auto check = [what, &field=this->grid_](const Map::Point& pt) {
        return field.at(pt).material == what && !field.at(pt).locked;
    };
    auto way = Map::findSmthOnTheField(
        where,
        this->grid_.size(),
        this->grid_.min(),
        std::move(cost),
        std::move(check)
    );
    return CurveToRelative(
        way,
        this->snake_.forward()
    );
}

}  // namespace Ant
