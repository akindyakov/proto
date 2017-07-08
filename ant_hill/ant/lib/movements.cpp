#include "movements.h"


namespace Ant {

bool discoverSomeSpace(
    Location& location
);

bool followTheWay(
    Location& location
    , const Map::RelativeDirectionCurve& way
);

bool followTheWayBack(
    Location& location
    , const Map::RelativeDirectionCurve& way
);

bool followTheWay(
    Location& location
    , const Map::RelativeDirectionCurve& way
) {
    for (const auto& to : way) {
        if (!location.frontMove(to)) {
            return false;
        }
    }
    return true;
}

bool followTheWayBack(
    Location& location
    , const Map::RelativeDirectionCurve& way
) {
    for (auto to : way) {
        if (to == Map::RelativeDirection::Forward()) {
            to = to.Inverse();
        }
        if (!location.backMove(to)) {
            return false;
        }
    }
    return true;
}

bool discoverSomeSpace(
    Location& location
) {
    while (true) {
        auto way = location.findMaterial(
            location.whereAmI(),
            Map::EMaterial::Unknown
        );
        if (!way.empty()) {
            followTheWay(location, way);
        } else {
            way = location.findMaterial(
                location.whereIsMyTail(),
                Map::EMaterial::Unknown
            );
            if (!way.empty()) {
                followTheWayBack(location, way);
            } else {
                return true;
            }
        }
    }
    return false;
}

void LookAroundTask::run(AntState& state) const {
    discoverSomeSpace(state.location);
}

}  // namespace Ant
