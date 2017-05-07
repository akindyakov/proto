#include "world.h"
#include "snake.h"

#include <tools/http_error.h>

#include <mutex>

namespace Map {

World::World(
    std::istream& fieldStream
)
    : field_(Map::ScanFromText<World::Cell>(fieldStream))
    , objects_(128, ObjectHash)
    , nextFreeId_(ObjectId::Invalid().id + 1)
{
}

ObjectId World::appear() {
    auto newId = ObjectId(nextFreeId_.fetch_add(1));
    std::lock_guard<std::mutex> lock(globalMutex);
    auto obj = std::make_shared<SnakeObj>(
        SnakeObj::appear(
            this->field_,
            {
                Map::EMaterial::AntBody,
                Map::EMaterial::AntHead,
            },
            newId
        )
    );
    objects_.insert(
        std::make_pair(newId, obj)
    );
    return newId;
}

void World::move(
    ObjectId id
    , Map::RelativeDirection direction
    , Side side
) {
    auto obj = World::findObject(id);
    std::lock_guard<std::mutex> lock(globalMutex);
    if (side == Side::Front) {
        obj->frontMove(
            this->field_,
            direction
        );
    } else {
        obj->backMove(
            this->field_,
            direction
        );
    }
}

void World::pickUpGrain(
    ObjectId id
    , Map::RelativeDirection direction
    , Side side
) {
    auto obj = World::findObject(id);
    std::lock_guard<std::mutex> lock(globalMutex);
    if (side == Side::Front) {
        obj->pushFrontGrain(
            this->field_,
            direction
        );
    } else {
        obj->pushBackGrain(
            this->field_,
            direction
        );
    }
}

void World::dropGrain(
    ObjectId id
    , Side side
) {
    auto obj = World::findObject(id);
    std::lock_guard<std::mutex> lock(globalMutex);
    if (side == Side::Front) {
        obj->popFrontGrain(
            this->field_
        );
    } else {
        obj->popBackGrain(
            this->field_
        );
    }
}

const World::Cell& World::lookTo(
    ObjectId id
    , RelativeDirection to
    , size_t segment
) const {
    auto obj = World::findObject(id);
    return obj->lookTo(
        this->field_,
        to,
        segment
    );
}

std::vector<RelativeDirection> World::getPose(
    ObjectId id
) const {
    auto obj = World::findObject(id);
    return obj->getPose();
}

void World::ping(
    ObjectId id
) const {
    World::findObject(id);
}

std::shared_ptr<IObject> World::findObject(
    ObjectId id
) const {
    auto objIt = objects_.find(id);
    if (objIt == objects_.end()) {
        throw NotFound() << "There is no object with such id " << id;
    }
    auto obj = objIt->second;
    if (obj.use_count() > 2) {
        throw LockedError() << "Object (" << id << ") is locked";
    }
    return obj;
}

void World::print(std::ostream& os) const {
    PrintToText(os, this->field_);
}

std::ostream& operator<<(std::ostream& os, const ObjectId& oid) {
    os << oid.id;
    return os;
}

std::istream& operator>>(std::istream& is, ObjectId& oid) {
    is >> oid.id;
    return is;
}

std::ostream& operator<<(std::ostream& os, const WorldCell& cell) {
    os << GetSymbolMap().GetSymbol(cell.grain);
    return os;
}

std::istream& operator>>(std::istream& is, WorldCell& cell) {
    auto ch = char{0};
    is.get(ch);
    cell.grain = GetSymbolMap().GetMaterial(ch);
    return is;
}

}
