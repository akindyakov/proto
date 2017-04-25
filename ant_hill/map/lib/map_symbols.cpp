#include "map_symbols.h"

#include <tools/tests/ut.h>

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
        {EMaterial::Invalid,      '?'},
    }
{
    for (const auto& p : MaterialToChar) {
        CharToMaterial.insert(std::make_pair(p.second, p.first));
    }
}

const EMaterial MapSymbols::GetMaterial(char ch) const {
    auto it = CharToMaterial.find(ch);
    if (it == CharToMaterial.end()) {
        throw AntHill::Exception("Unexpected material character")
            << "[" << ch << "]";
    }
    return it->second;
}

const char MapSymbols::GetSymbol(EMaterial m) const {
    auto it = MaterialToChar.find(m);
    if (it == MaterialToChar.end()) {
        throw AntHill::Exception("Unexpected material code")
            << "[" << static_cast<int>(m) << "]";
    }
    // std::cerr << "[" << it->second << "]\n";
    return it->second;
}
