#include "world.h"

namespace Map {

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
