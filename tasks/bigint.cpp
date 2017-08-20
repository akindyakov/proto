#include <iostream>
#include <limits>
#include <cstdint>
#include <vector>
#include <iomanip>

class BigInt
{
public:
    using DigitType = std::uint32_t;
    using UpType = std::uint64_t;
    static constexpr UpType Base = 1000000000;

public:
    explicit BigInt(
        int t
    )
        : neg(t < 0)
    {
        num.push_back(std::abs(t));
    }

    void multiply(int value) {
        neg = ((value < 0) != neg);
        auto uval = static_cast<UpType>(std::abs(value));
        auto rem = UpType{0};
        for (auto& digit : num) {
            auto newDigit = uval * static_cast<UpType>(digit) + rem;
            rem = newDigit / Base;
            digit = static_cast<DigitType>(newDigit % Base);
        }
        if (0 != rem) {
            num.push_back(
                static_cast<DigitType>(rem)
            );
        }
    }

    void print(std::ostream& os) const {
        if (neg) {
            os << '-';
        }
        auto it = num.rbegin();
        os << *it;
        ++it;
        for (; it < num.rend(); ++it) {
            os << std::setfill('0') << std::setw(9) << *it;
        }
    }

public:
    std::vector<std::uint32_t> num;
    bool neg = false;
};

int main() {
    auto num = int{0};
    std::cin >> num;
    auto b = BigInt(1);
    for (int v = 1; v <= num; v++) {
        b.multiply(v);
    }
    b.print(std::cout);
}
