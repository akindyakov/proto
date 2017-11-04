#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


bool nextPermutation(std::string& str) {
    size_t pos = str.size();
    if (pos == 0) {
        return false;
    }
    --pos;
    while (pos != 0 && !(str[pos - 1] < str[pos])) {
        --pos;
    }
    if (pos == 0) {
        return false;
    }
    size_t min = pos;
    --pos;
    for (size_t i = min; i < str.size(); ++i) {
        if (str[pos] < str[i] && str[i] < str[min]) {
            min = i;
        }
    }
    std::swap(str[min], str[pos]);
    std::sort(str.begin() + pos + 1, str.end());
    return true;
}

std::vector<std::string> allPermutations(std::string str) {
    std::sort(str.begin(), str.end());
    std::vector<std::string> ans;
    ans.push_back(str);
    while (nextPermutation(str)) {
        ans.push_back(str);
    }
    return ans;
}

//void test() {
//    if (allPermutations("ABC") != std::vector<std::string>({
//        "ABC", "ACB", "BAC", "BCA", "CAB", "CBA"
//    })) {
//        std::cerr << "0: failed\n";
//    }
//    if (allPermutations("SGBA") != std::vector<std::string>({
//        "ABGS", "ABSG", "AGBS", "AGSB", "ASBG", "ASGB", "BAGS", "BASG", "BGAS", "BGSA", "BSAG", "BSGA", "GABS", "GASB", "GBAS", "GBSA", "GSAB", "GSBA", "SABG", "SAGB", "SBAG", "SBGA", "SGAB", "SGBA"})) {
//        std::cerr << "1: failed\n";
//    }
//}

int main() {
    //test();
    std::size_t n(0);
    std::cin >> n;
    while (n--) {
        std::string str;
        std::cin >> str;
        std::sort(str.begin(), str.end());
        std::cout << str << " ";
        if (str.size() < 2) {
            std::cout << "\n";
            continue;
        }
        while (nextPermutation(str)) {
            std::cout << str << " ";
        }
        std::cout << "\n";
    }
}
