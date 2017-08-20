#include <iostream>
#include <vector>
#include <algorithm>

/**
* std::sort(first.begin(), first.end());
* std::sort(second.begin(), second.end());
* std::sort(third.begin(), third.end());
*/

size_t doubleCount(
    int x
    , const std::vector<int>& first
    , const std::vector<int>& second
) {
    auto counter = size_t{0};
    auto fit = first.begin();
    auto sit = second.rbegin();
    while (fit != first.end() && sit != second.rend()) {
        if (*fit + *sit > x) {
            ++sit;
        } else if (*fit + *sit < x) {
            ++fit;
        } else {
            auto fnext = std::find_if(fit, first.end(), [fit](int value) { return value != *fit; });
            auto snext = std::find_if(sit, second.rend(), [sit](int value) { return value != *sit; });
            counter += std::distance(fit, fnext) * std::distance(sit, snext);
            fit = fnext;
            sit = snext;
        }
    }
    return counter;
}

size_t tripleCount(
    int x
    , const std::vector<int>& first
    , const std::vector<int>& second
    , const std::vector<int>& third
) {
    auto counter = size_t{0};
    if (third.empty()) {
        return counter;
    }
    auto tPrev = third.front() + third.front();
    auto prevAdd = size_t{0};
    for (auto t : third) {
        if (t != tPrev) {
            prevAdd = doubleCount(x - t, first, second);
            tPrev = t;
        }
        counter += prevAdd;
    }
    return counter;
}

template<
    typename F
    , typename S
>
void test_equal(int n, F f, S s) {
    if (f == s) {
        std::cout << n << ": passed\n";
    } else {
        std::cerr << n << ": failed " << f << " is not equal " << s << "\n";
    }
}

int main() {
    test_equal(0, 1, tripleCount(3, {1, 2, 3}, {1, 2, 3}, {1, 2, 3}));
    test_equal(1, 3, tripleCount(3, {1, 1, 1}, {1, 2, 3}, {1, 2, 3}));
    test_equal(2, 12, tripleCount(3, {1, 1, 1}, {1, 1, 3}, {1, 1, 3}));
    test_equal(3, 27, tripleCount(3, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}));
    test_equal(4, 27, tripleCount(1, {-1, -1, -1}, {1, 1, 1}, {1, 1, 1}));
    test_equal(5, 6, tripleCount(1, {0, 0, -1}, {-1, 0, 1}, {1, 1, 1}));
    test_equal(6, 6, tripleCount(2, {0, 0, -1}, {-1, 0, 1}, {1, 1, 1}));
}
