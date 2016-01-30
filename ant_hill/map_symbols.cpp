#include "map_symbols.h"

const TMapSymbols& GetSymbolMap() {
    static TMapSymbols m;
    return m;
}

TMapSymbols::TMapSymbols()
    : MaterialToChar{
        {EMaterial::EmptySpace,   ' '},
        {EMaterial::Sand,         's'},
        {EMaterial::Marble,       'm'},
        {EMaterial::Iron,         'i'},
        {EMaterial::Wood,         'w'},
        {EMaterial::AntHead,      'V'},
        {EMaterial::AntBody,      '#'},
        {EMaterial::Invalid,      '?'},
    }
{
    for (const auto& p : MaterialToChar) {
        CharToMaterial.insert(std::make_pair(p.second, p.first));
    }
}

const EMaterial TMapSymbols::GetMaterial(char ch) const {
    auto it = CharToMaterial.find(ch);
    if (it == CharToMaterial.end()) {
        throw std::exception();
    }
    return it->second;
}

const char TMapSymbols::GetSymbol(EMaterial m) const {
    auto it = MaterialToChar.find(m);
    if (it == MaterialToChar.end()) {
        throw std::exception();
    }
    return it->second;
}
