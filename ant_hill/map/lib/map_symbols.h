#pragma once

#include <vector>
#include <iostream>
#include <map>

namespace Map {

enum class EMaterial {
    EmptySpace,
    Sand,
    Marble,
    Iron,
    Wood,
    Loam,
    Water,
    AntHead,
    AntBody,
    Unknown
};

class MapSymbols {
public:
    MapSymbols();

    MapSymbols(const MapSymbols&) = delete;
    MapSymbols(MapSymbols&&) = delete;
    MapSymbols& operator=(const MapSymbols&) = delete;
    MapSymbols& operator=(MapSymbols&&) = delete;

    const EMaterial GetMaterial(char ch) const;
    const char GetSymbol(EMaterial m) const;

private:
    static const char FirstChar = ' ';

    std::map<EMaterial, char> MaterialToChar;
    std::map<char, EMaterial> CharToMaterial;
};

const MapSymbols& GetSymbolMap();

}  // namespace Map
