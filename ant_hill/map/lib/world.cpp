#include "world.h"
#include "snake.h"

#include <lib/http_error.h>

#include <mutex>

namespace Map {

World::SnakeType createSnakeOnField(
    World::FieldType& where
    , const std::vector<EMaterial>& body
    , ObjectId id
) {
    /**
    // сделай тут тупой поиск свободных мест нужного размера без всяких цепочек
    */
    auto chain = std::vector<RelativeDirection>(
        body.size() - 1,
        RelativeDirection::Forward()
    );
    auto start = Point{0, 0};
    for (start.Y = where.min().Y; start.Y < where.max().Y; ++start.Y) {
        for (start.X = where.min().X; start.X < where.max().X; ++start.X) {
            auto startDir = Direction::North();
            while (startDir.counter() == 0) {
                bool vacant = true;
                if (where.inRange(start) && where.at(start).isFree()) {
                    auto pt = start;
                    for (auto& el : chain) {
                        pt = el.Turn(startDir).MovePoint(pt);
                        if (!where.inRange(pt) || !where.at(pt).isFree()) {
                            vacant = false;
                            break;
                        }
                    }
                } else {
                    vacant = false;
                }
                if (vacant) {
                    auto materialIt = body.begin();
                    auto& cell = where.at(start);
                    cell.objectId = id;
                    cell.grain = *materialIt;
                    ++materialIt;
                    auto tail = std::vector<Direction>{};
                    auto pt = start;
                    for (auto& el : chain) {
                        auto from = el.Turn(startDir);
                        tail.push_back(from);
                        pt = from.MovePoint(pt);
                        auto& cell = where.at(pt);
                        cell.objectId = id;
                        cell.grain = *materialIt;
                        ++materialIt;
                    }
                    return World::SnakeType(pt, tail);
                }
                ++startDir;
            }
        }
    }
    throw InternalServerError() << "There is no vacant position";
}


World::World(
    std::istream& fieldStream
)
    : field_(
        ScanFromText<World::CellType>(
            fieldStream
        )
    )
    , objects_(128, ObjectHash)
    , nextFreeId_(ObjectId::Invalid().id + 1)
{
}

ObjectId World::appear() {
    auto newId = ObjectId(nextFreeId_.fetch_add(1));
    std::lock_guard<std::mutex> lock(globalMutex);
    auto obj = std::make_shared<SnakeType>(
        createSnakeOnField(
            this->field_,
            {
                EMaterial::AntBody,
                EMaterial::AntHead,
            },
            newId
        )
    );
    objects_.insert(
        std::make_pair(newId, std::move(obj))
    );
    return newId;
}

void World::move(
    ObjectId id
    , RelativeDirection direction
    , Side side
) {
    auto obj = World::findObject(id);
    std::lock_guard<std::mutex> lock(globalMutex);
    if (side == Side::Front) {
        auto pt = obj->lookTo(direction, 0);
        if (!this->field_.inRange(pt) || !this->field_.at(pt).isFree()) {
            throw Forbidden() << "There is no free space for front move.";
        }
        obj->frontMove(
            this->field_,
            direction
        );
    } else {
        auto pt = obj->lookTo(direction, obj->size() - 1);
        if (!this->field_.inRange(pt) || !this->field_.at(pt).isFree()) {
            throw Forbidden() << "There is no free space for back move.";
        }
        obj->backMove(
            this->field_,
            direction
        );
    }
}

void World::pickUpGrain(
    ObjectId id
    , RelativeDirection direction
) {
    auto obj = World::findObject(id);
    std::lock_guard<std::mutex> lock(globalMutex);

    auto pt = obj->pushFrontGrain(direction);
    auto& cell = this->field_.at(pt);
    if (cell.objectId.isValid()) {
        obj->popFrontGrain();
        throw Forbidden() << "This grain belongs to other ant. Theft is outlow!";
    }
    cell.objectId = id;
}

void World::dropGrain(
    ObjectId id
) {
    auto obj = World::findObject(id);
    std::lock_guard<std::mutex> lock(globalMutex);

    auto pt = obj->popFrontGrain();
    this->field_.at(pt).objectId = ObjectId::Invalid();
}

const World::CellType& World::lookTo(
    ObjectId id
    , RelativeDirection to
    , size_t segment
) const {
    auto obj = World::findObject(id);
    auto pt = obj->lookTo(to, segment);
    static const auto forbidden = CellType{EMaterial::Forbidden};
    if (!this->field_.inRange(pt)) {
        return forbidden;
    }
    return this->field_.at(pt);
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

std::shared_ptr<World::SnakeType> World::findObject(
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
