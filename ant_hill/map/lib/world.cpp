#include "world.h"

namespace Map {

std::ostream& operator<<(std::ostream& os, const Map::WorldCell& cell) {
    os << GetSymbolMap().GetSymbol(cell.grain);
    return os;
}

std::istream& operator>>(std::istream& is, Map::WorldCell& cell) {
    auto ch = char{0};
    is.get(ch);
    cell.grain = GetSymbolMap().GetMaterial(ch);
    return is;
}

}
