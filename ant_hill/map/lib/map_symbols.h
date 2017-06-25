#pragma once

#include <vector>
#include <iostream>
#include <map>

namespace Map {

enum class EMaterial {
    EmptySpace,
    Unknown,
    AntHead,
    AntBody,
    Attention,
    DoubleQuote,
    EndOfLine,
    Div,
    And,
    LeftParent,
    RightParent,
    Multi,
    Coma,
    Minus,
    Slash,
    Digit0,
    Digit1,
    Digit2,
    Digit3,
    Digit4,
    Digit5,
    Digit6,
    Digit7,
    Digit8,
    Digit9,
    DotDot,
    DotAndComa,
    Less,
    Equal,
    Greater,
    Dog,
    LetterA,
    LetterB,
    LetterC,
    LetterD,
    LetterE,
    LetterF,
    LetterG,
    LetterH,
    LetterI,
    LetterJ,
    LetterK,
    LetterL,
    LetterM,
    LetterN,
    LetterO,
    LetterP,
    LetterQ,
    LetterR,
    LetterS,
    LetterT,
    LetterU,
    LetterV,
    LetterW,
    LetterX,
    LetterY,
    LetterZ,
    QLeft,
    Quote,
    BackSlash,
    QRight,
    Roof,
    Floor,
    BackQuote,
    Letter_a,
    Letter_b,
    Letter_c,
    Letter_d,
    Letter_e,
    Letter_f,
    Letter_g,
    Letter_h,
    Letter_i,
    Letter_j,
    Letter_k,
    Letter_l,
    Letter_m,
    Letter_n,
    Letter_o,
    Letter_p,
    Letter_q,
    Letter_r,
    Letter_s,
    Letter_t,
    Letter_u,
    Letter_v,
    Letter_w,
    Letter_x,
    Letter_y,
    Letter_z,
    LeftBrace,
    Pipe,
    RightBrace,
    Tilda,
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

std::ostream& operator<<(std::ostream& is, const EMaterial& material);
std::istream& operator>>(std::istream& is, EMaterial& material);

}  // namespace Map
