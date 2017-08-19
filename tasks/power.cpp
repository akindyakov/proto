#include <iostream>

template<
    typename T
>
T power(T value, int p) {
    auto inv = p < 0;
    p *= inv ? -1 : 1;
    auto ans = T{1};
    auto up = static_cast<unsigned>(p);
    while (up) {
        if (up & 1) {
            ans *= value;
        }
        value *= value;
        up >>= 1;
    }
    if (inv) {
        ans = 1 / ans;
    }
    return ans;
}

template<
    typename T
>
T rec_power_impl(T value, int p) {
    if (p == 0) {
        return T{1};
    }
    auto ans = rec_power_impl(value, p/2);
    ans *= ans;
    if (p % 2) {
        ans *= value;
    }
    return ans;
}

template<
    typename T
>
T rec_power(T value, int p) {
    auto inv = p < 0;
    p *= inv ? -1 : 1;
    auto ans = rec_power_impl(value, p);
    if (inv) {
        ans = 1 / ans;
    }
    return ans;
}

template<
    typename T
>
T simple(T value, int p) {
    auto inv = p < 0;
    p *= inv ? -1 : 1;
    auto ans = T{1};
    while (p--) {
        ans *= value;
    }
    return ans;
}


int main() {
    for (int v = 0; v < 100; ++v) {
        for (int p = 0; p < 10; ++p) {
            if (power(v, p) != simple(v, p)) {
                std::cerr << "power: Wrong answer: " << power(v, p) << ". Expected: " << simple(v, p) << std::endl;
            }
            if (rec_power(v, p) != simple(v, p)) {
                std::cerr << "rec_power: Wrong answer: " << rec_power(v, p) << ". Expected: " << simple(v, p) << std::endl;
            }
            std::cerr << v << ". " << p << std::endl;
        }
    }
}
