#include "map_symbols.h"

#include <tools/exception.h>

namespace Map {

const MapSymbols& GetSymbolMap() {
    static MapSymbols m;
    return m;
}

MapSymbols::MapSymbols()
    : MaterialToChar{
        {EMaterial::EmptySpace,   '.'},
        {EMaterial::Sand,         's'},
        {EMaterial::Marble,       'm'},
        {EMaterial::Iron,         'i'},
        {EMaterial::Wood,         'w'},
        {EMaterial::Loam,         'l'},
        {EMaterial::Water,        'a'},
        {EMaterial::AntHead,      'X'},
        {EMaterial::AntBody,      '#'},
        {EMaterial::Unknown,      '?'},
        {EMaterial::Forbidden,    'F'},
    }
{
    for (const auto& p : MaterialToChar) {
        CharToMaterial.insert(std::make_pair(p.second, p.first));
    }
}

const EMaterial MapSymbols::GetMaterial(char ch) const {
    auto it = CharToMaterial.find(ch);
    if (it == CharToMaterial.end()) {
        throw Exception("Unexpected material character")
            << "[" << ch << "]";
    }
    return it->second;
}

const char MapSymbols::GetSymbol(EMaterial m) const {
    auto it = MaterialToChar.find(m);
    if (it == MaterialToChar.end()) {
        throw Exception("Unexpected material code")
            << "[" << static_cast<int>(m) << "]";
    }
    return it->second;
}

std::ostream& operator<<(std::ostream& os, const EMaterial& material) {
    os << GetSymbolMap().GetSymbol(material);
    return os;
}

std::istream& operator>>(std::istream& is, EMaterial& material) {
    auto ch = char{0};
    is.get(ch);
    material = GetSymbolMap().GetMaterial(ch);
    return is;
}

}  // namespace Map
