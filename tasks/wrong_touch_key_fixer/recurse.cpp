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

void nearbyWordsRec(
    std::vector<std::string>& ret
    , std::size_t index
    , std::string& word
    , const std::string& source
) {
    if (index < source.size()) {
        auto nearby = getNearbyChars(source[index]);
        for (const auto ch : nearby) {
            word[index] = ch;
            nearbyWordsRec(ret, index + 1, word, source);
        }
    } else {
        if (isWord(word)) {
            ret.push_back(word);
        }
    }
}

std::vector<std::string>
nearbyWords(
    const std::string& source
) {
    auto ret = std::vector<std::string>{};
    auto word = source;
    nearbyWordsRec(ret, 0, word, source);
    return ret;
}

int
main() {
    for (const auto& p : nearbyWords("gi")) {
        std::cout << p << '\n';
    }
}
