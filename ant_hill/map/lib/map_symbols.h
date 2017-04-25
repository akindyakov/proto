#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

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
    Invalid
};


namespace std {
template<>
struct hash<EMaterial>
{
    size_t operator()(const EMaterial e) const
    {
        return static_cast<size_t>(e);
    }
};
}

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

    std::unordered_map<EMaterial, char> MaterialToChar;
    std::unordered_map<char, EMaterial> CharToMaterial;
};

const MapSymbols& GetSymbolMap();
