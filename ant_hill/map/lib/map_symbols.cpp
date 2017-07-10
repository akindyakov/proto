#include "map_symbols.h"

#include <lib/exception.h>

namespace Map {

const MapSymbols& GetSymbolMap() {
    static MapSymbols m;
    return m;
}

MapSymbols::MapSymbols()
    : MaterialToChar{
        {EMaterial::EmptySpace, '.'},
        {EMaterial::Unknown, '?'},
        {EMaterial::AntHead, '+'},
        {EMaterial::AntBody, '#'},
        {EMaterial::Attention, '!'},
        {EMaterial::DoubleQuote, '"'},
        {EMaterial::EndOfLine, '$'},
        {EMaterial::Div, '%'},
        {EMaterial::And, '&'},
        {EMaterial::LeftParent, '('},
        {EMaterial::RightParent, ')'},
        {EMaterial::Multi, '*'},
        {EMaterial::Coma, ','},
        {EMaterial::Minus, '-'},
        {EMaterial::Slash, '/'},
        {EMaterial::Digit0, '0'},
        {EMaterial::Digit1, '1'},
        {EMaterial::Digit2, '2'},
        {EMaterial::Digit3, '3'},
        {EMaterial::Digit4, '4'},
        {EMaterial::Digit5, '5'},
        {EMaterial::Digit6, '6'},
        {EMaterial::Digit7, '7'},
        {EMaterial::Digit8, '8'},
        {EMaterial::Digit9, '9'},
        {EMaterial::DotDot, ':'},
        {EMaterial::DotAndComa, ';'},
        {EMaterial::Less, '<'},
        {EMaterial::Equal, '='},
        {EMaterial::Greater, '>'},
        {EMaterial::Dog, '@'},
        {EMaterial::LetterA, 'A'},
        {EMaterial::LetterB, 'B'},
        {EMaterial::LetterC, 'C'},
        {EMaterial::LetterD, 'D'},
        {EMaterial::LetterE, 'E'},
        {EMaterial::LetterF, 'F'},
        {EMaterial::LetterG, 'G'},
        {EMaterial::LetterH, 'H'},
        {EMaterial::LetterI, 'I'},
        {EMaterial::LetterJ, 'J'},
        {EMaterial::LetterK, 'K'},
        {EMaterial::LetterL, 'L'},
        {EMaterial::LetterM, 'M'},
        {EMaterial::LetterN, 'N'},
        {EMaterial::LetterO, 'O'},
        {EMaterial::LetterP, 'P'},
        {EMaterial::LetterQ, 'Q'},
        {EMaterial::LetterR, 'R'},
        {EMaterial::LetterS, 'S'},
        {EMaterial::LetterT, 'T'},
        {EMaterial::LetterU, 'U'},
        {EMaterial::LetterV, 'V'},
        {EMaterial::LetterW, 'W'},
        {EMaterial::LetterX, 'X'},
        {EMaterial::LetterY, 'Y'},
        {EMaterial::LetterZ, 'Z'},
        {EMaterial::QLeft, '['},
        {EMaterial::Quote, '\''},
        {EMaterial::BackSlash, '\\'},
        {EMaterial::QRight, ']'},
        {EMaterial::Roof, '^'},
        {EMaterial::Floor, '_'},
        {EMaterial::BackQuote, '`'},
        {EMaterial::Letter_a, 'a'},
        {EMaterial::Letter_b, 'b'},
        {EMaterial::Letter_c, 'c'},
        {EMaterial::Letter_d, 'd'},
        {EMaterial::Letter_e, 'e'},
        {EMaterial::Letter_f, 'f'},
        {EMaterial::Letter_g, 'g'},
        {EMaterial::Letter_h, 'h'},
        {EMaterial::Letter_i, 'i'},
        {EMaterial::Letter_j, 'j'},
        {EMaterial::Letter_k, 'k'},
        {EMaterial::Letter_l, 'l'},
        {EMaterial::Letter_m, 'm'},
        {EMaterial::Letter_n, 'n'},
        {EMaterial::Letter_o, 'o'},
        {EMaterial::Letter_p, 'p'},
        {EMaterial::Letter_q, 'q'},
        {EMaterial::Letter_r, 'r'},
        {EMaterial::Letter_s, 's'},
        {EMaterial::Letter_t, 't'},
        {EMaterial::Letter_u, 'u'},
        {EMaterial::Letter_v, 'v'},
        {EMaterial::Letter_w, 'w'},
        {EMaterial::Letter_x, 'x'},
        {EMaterial::Letter_y, 'y'},
        {EMaterial::Letter_z, 'z'},
        {EMaterial::LeftBrace, '{'},
        {EMaterial::Pipe, '|'},
        {EMaterial::RightBrace, '}'},
        {EMaterial::Tilda, '~'},
    }
{
    for (const auto& p : MaterialToChar) {
        CharToMaterial.insert(std::make_pair(p.second, p.first));
    }
}

const EMaterial MapSymbols::GetMaterial(char ch) const {
    auto it = CharToMaterial.find(ch);
    if (it == CharToMaterial.end()) {
        throw Lib::Exception("Unexpected material character")
            << "[" << ch << "]";
    }
    return it->second;
}

const char MapSymbols::GetSymbol(EMaterial m) const {
    auto it = MaterialToChar.find(m);
    if (it == MaterialToChar.end()) {
        throw Lib::Exception("Unexpected material code")
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
