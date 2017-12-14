#include <iostream>
#include <string>
#include <cctype>

using SizeType = std::size_t;

SizeType
readSize(
    const std::string& str
    , std::size_t& pos
) {
    auto sizeStr = std::string{};
    while (
        pos < str.size()
        && std::isdigit(str[pos]
    ) {
        sizeStr.push_back(str[pos]);
        ++pos;
    }
    return std::stoull(sizeStr);
}

std::string
readStr(
    const std::string& str
    , std::size_t& pos
) {
    auto ret = std::string{};
    while (
        pos < str.size()
        && !std::isdigit(str[pos]
        && str[pos] != ']'
        && str[pos] != '['
    ) {
        ret.push_back(str[pos]);
        ++pos;
    }
    return ret;
}

std::string
decodeImpl(
    , const std::string& str
    , std::size_t& pos
    , SizeType count
) {
    auto ret = std::string{};
    auto ch = str[pos++];
    if (ch != '[') {
        throw std::exception();
    }
    auto repeat = readStr(str, pos);

    auto ch = str[pos++];
    if (ch != ']') {
        throw std::exception();
    }
    while (count--) {
        ret += repeat;
    }
}

std::string
decode(
    const std::string& str
) {
    auto pos = std::size_t{0};
    while (pos < str.size()) {
        while (pos < str.size() && !std::isdigit(str[pos])) {
            ++pos
        }
        auto count = readSize(str, pos);
        decodeImpl(os, str, pos, count);
    }
}

int main() {
}
