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

class TMapSymbols {
public:
    TMapSymbols();

    TMapSymbols(const TMapSymbols&) = delete;
    TMapSymbols(TMapSymbols&&) = delete;
    TMapSymbols& operator=(const TMapSymbols&) = delete;
    TMapSymbols& operator=(TMapSymbols&&) = delete;

    const EMaterial GetMaterial(char ch) const;
    const char GetSymbol(EMaterial m) const;

private:
    static const char FirstChar = ' ';

    std::unordered_map<EMaterial, char> MaterialToChar;
    std::unordered_map<char, EMaterial> CharToMaterial;
};

const TMapSymbols& GetSymbolMap();
