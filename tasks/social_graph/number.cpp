#include <iostream>

template<
    typename Follows
>
int famous(int N, Follows follows = Follows{}) {
    int famous = 1;
    for (int next = 2; next <= N; ++next) {
        if (!follows(next, famous)) {
            famous = next;
        }
    }
    for (int i = 1; i <= N; ++i) {
        if (i == famous) {
            continue;
        }
        if (follows(famous, i) || !follows(i, famous)) {
            return 0;
        }
    }
    return famous;
}

int main() {
    auto ans = famous(10, [](int from, int to) { return to == 5; });
    if (5 != ans) {
        std::cerr << "0: error " << ans << ", expected 5\n";
    }
    ans = famous(10000, [](int from, int to) { return to == 100; });
    if (100 != ans) {
        std::cerr << "0: error " << ans << ", expected 100\n";
    }
    ans = famous(10000, [](int from, int to) { return from != 100; });
    if (100 != ans) {
        std::cerr << "0: error " << ans << ", expected 100\n";
    }
}
