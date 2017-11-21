#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


std::string
getNearbyChars(
    std::string::value_type ch
) {  // O(1)
    static const auto m = std::unordered_map<
        std::string::value_type,
        std::string
    > {
        {'g', "fgh"},
        {'i', "iko"},
    };
    return m.at(ch);
}


bool
isWord(
    const std::string& word
) { // O(1)
    if (word == "ok" || word == "hi") {
        return true;
    }
    return false;
}

void resetRange(
    std::size_t begin
    , std::string& word
    , const std::string& source
) {
    for (auto i = begin; i < word.size(); ++i) {
        word[i] = getNearbyChars(source[i])[0];
    }
}

bool nextPermutation(
    std::string& word
    , const std::string& source
) {
    for (std::size_t i = word.size(); i-- > 0;) {
        auto nearby = getNearbyChars(source[i]);
        if (word[i] != nearby.back()) {
            std::size_t j = 0;
            for (; j < nearby.size(); ++j) {
                if (word[i] == nearby[j]) {
                    break;
                }
            }
            word[i] = nearby[j + 1];
            resetRange(i + 1, word, source);
            return true;
        }
    }
    return false;
}

std::vector<std::string>
nearbyWords(
    const std::string& word
) {
    auto ret = std::vector<std::string>{};
    if (word.empty()) {
        return ret;
    }
    auto current = word;
    resetRange(0, current, word);
    if (isWord(current)) {
        ret.push_back(current);
    }
    while (nextPermutation(current, word)) {
        if (isWord(current)) {
            ret.push_back(current);
        }
    }
    return ret;
}

int main() {
    for (const auto& p : nearbyWords("gi")) {
        std::cout << p << '\n';
    }
}
