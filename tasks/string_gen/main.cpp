#include <algorithm>
#include <iostream>
#include <string>

using UintType = unsigned long long;

bool permutations(
    std::string& str
) {
    auto pos = str.size() - 1;
    while (pos && !(str[pos - 1] < str[pos])) {
        --pos;
    }
    if (pos == 0) {
        return false;
    }
    auto min = pos;
    --pos;
    for (auto i = min; i < str.size(); ++i) {
        if (str[i] > str[pos] && str[i] < str[min]) {
            min = i;
        }
    }
    std::swap(str[pos], str[min]);
    std::sort(str.begin() + pos + 1, str.end());
    return true;
}

UintType countPermutations(
    std::string str
) {
    auto ans = UintType{1};
    if (str.size() < 2) {
        return ans;
    }
    std::sort(str.begin(), str.end());
    while (permutations(str)) {
        ++ans;
    }
    return ans;
}

int test2() {
    auto ans = countPermutations("");
    if (ans != 1) {
        std::cerr << "2: wrong answer: " << ans << ", expected 1\n";
        return 1;
    }
    ans = countPermutations("1");
    if (ans != 1) {
        std::cerr << "2: wrong answer: " << ans << ", expected 1\n";
        return 1;
    }
    ans = countPermutations("12");
    if (ans != 2) {
        std::cerr << "2: wrong answer: " << ans << ", expected 1\n";
        return 1;
    }
    return 0;
}

int test0() {
    auto str = std::string{"aac"};
    auto ans = std::vector<std::string>{str,};
    while (permutations(str)) {
        ans.push_back(str);
    }
    if (ans != std::vector<std::string>{"aac", "aca", "caa"}) {
        std::cerr << "0: fail ";
        for (const auto& item : ans) {
            std::cerr << "(" << item << "),";
        }
        std::cerr << '\n';
        return 1;
    }
    return 0;
}

int test1() {
    auto str = std::string{"abc"};
    auto ans = std::vector<std::string>{str,};
    while (permutations(str)) {
        ans.push_back(str);
    }
    if (ans != std::vector<std::string>{"abc", "acb", "bac", "bca", "cab", "cba"}) {
        std::cerr << "1: fail ";
        for (const auto& item : ans) {
            std::cerr << "(" << item << "),";
        }
        std::cerr << '\n';
        return 1;
    }
    return 0;
}

UintType notRepeatedCombinations(
    UintType n
    , UintType k
) {
    auto ans = UintType{1};
    for (UintType i = k + 1; i < n + k; ++i) {
        ans *= i;
    }
    for (UintType i = 1; i < n; ++i) {
        ans /= i;
    }
    return ans;
}

int test3() {
    auto ans = notRepeatedCombinations(1, 2);
    if (ans != 1) {
        std::cerr << "3: " << ans << " != 1\n";
        return 1;
    }
    ans = notRepeatedCombinations(2, 2);
    if (ans != 3) {
        std::cerr << "3: " << ans << " != 3\n";
        return 1;
    }
    return 0;
}

int main() {
    //return test0() + test1() + test2() + test3();
    auto prefixes = std::vector<std::string>{"", "b", "c", "cc", "bc", "bcc"};
    auto t = UintType{0};
    std::cin >> t;
    for (auto i = t; i--;) {
        auto n = UintType{0};
        std::cin >> n;
        auto count = UintType{0};
        for (auto str : prefixes) {
            if (str.size() > n) {
                break;
            }
            auto p = countPermutations(str);
            auto c = notRepeatedCombinations(str.size() + 1, n - str.size());
            count += p * c;
        }
        std::cout << count << '\n';
    }
}
