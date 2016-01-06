#pragma once

#include <vector>

enum class EMaterial {
    EmptySpace,
    Sand,
    Marble,
    Iron,
    Wood,
    AntHead,
    AntMesosoma,
    AntPetiole,
    AntGaster,
    Invalid
};


class TMapSymbols {
public:
    TMapSymbols()
        : CharToMaterial(128, EMaterial::Invalid)
    {
        MaterialToChar = {
            ' ', // EmptySpace,
            's', // Sand
            'm', // Marble
            'i', // Iron
            'w', // Wood
            'V', // AntHead
            '8', // AntMesosoma
            '.', // AntPetiole
            '@', // AntGaster
            '?'  // Invalid
        };
        for (std::size_t i = 0; i < MaterialToChar.size(); ++i) {
            auto index = static_cast<std::size_t>(MaterialToChar[i] - FirstChar);
            CharToMaterial[index] = static_cast<EMaterial>(i);
        }
    }
    TMapSymbols(const TMapSymbols&) = delete;
    TMapSymbols(TMapSymbols&&) = delete;
    TMapSymbols& operator=(const TMapSymbols&) = delete;
    TMapSymbols& operator=(TMapSymbols&&) = delete;

    const EMaterial GetMaterial(char ch) {
        return CharToMaterial[static_cast<std::size_t>(ch)];
    }

    const char GetSymbol(EMaterial m) {
        return MaterialToChar[static_cast<std::size_t>(m)];
    }

private:
    static const char FirstChar = ' ';
    std::vector<char> MaterialToChar;
    std::vector<EMaterial> CharToMaterial;
};

const TMapSymbols& GetSymbolMap() {
    static TMapSymbols m;
    return m;
}
