#include <cstddef>
#include <iostream>
#include <vector>


/**
 * There is a array of prices (float) for every considerable day.
 * Function should determine the best day of sale and day of buy,
 * which will make more money.
 * For instance for the array [0.3, 1.1, 0.2, 0.5, 1.2, 1.0, 0.1,]
 * the bets trade interval is [2, 4]. 2 is the buy day and 4 sale day.
 */

struct TraidInterval {
    std::size_t buyDay = 0u;
    std::size_t saleDay = 0u;
    float priceDifference = -0.f;
};

TraidInterval solution(
    const std::vector<float>& prices
) {
    auto best = TraidInterval();
    auto current = TraidInterval();
    for (auto i = std::size_t{0}; i < prices.size(); ++i) {
        if (prices[current.saleDay] < prices[i]) {
            current.saleDay = i;
            current.priceDifference = prices[i] - prices[current.buyDay];
        }
        if (prices[i] < prices[current.buyDay]) {
            if (best.priceDifference < current.priceDifference) {
                best = current;
            }
            current.priceDifference = 0.f;
            current.buyDay = i;
            current.saleDay = i;
        }
    }
    if (best.priceDifference < current.priceDifference) {
        best = current;
    }
    return best;
}

int main() {
    {
        auto interval = solution({0.3, 1.1, 0.2, 0.5, 1.2, 1.0, 0.1});
        if (interval.buyDay != 2) {
            std::cerr << "0.0 failure " << interval.buyDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
        if (interval.saleDay != 4) {
            std::cerr << "0.1 failure " << interval.saleDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
    }
    {
        auto interval = solution({3.0, 2.0, 1.0});
        if (interval.buyDay != 0) {
            std::cerr << "1.0 failure " << interval.buyDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
        if (interval.saleDay != 0) {
            std::cerr << "1.1 failure " << interval.saleDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
    }
    {
        auto interval = solution({3.0, 1.0, 2.0, 3.0});
        if (interval.buyDay != 1) {
            std::cerr << "2.0 failure " << interval.buyDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
        if (interval.saleDay != 3) {
            std::cerr << "2.1 failure " << interval.saleDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
    }
    {
        auto interval = solution({});
        if (interval.buyDay != 0) {
            std::cerr << "3.0 failure " << interval.buyDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
        if (interval.saleDay != 0) {
            std::cerr << "3.1 failure " << interval.saleDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
    }
    {
        auto interval = solution({0.2, 0.3, 5.0, 0.1, 4.0});
        if (interval.buyDay != 0) {
            std::cerr << "4.0 failure " << interval.buyDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
        if (interval.saleDay != 2) {
            std::cerr << "4.1 failure " << interval.saleDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
    }
    {
        auto interval = solution({0.2, 0.3, 5.0, 0.1});
        if (interval.buyDay != 0) {
            std::cerr << "5.0 failure " << interval.buyDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
        if (interval.saleDay != 2) {
            std::cerr << "5.1 failure " << interval.saleDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
    }
    {
        auto interval = solution({0.2, 0.3, 5.0, 0.1});
        if (interval.buyDay != 0) {
            std::cerr << "6.0 failure " << interval.buyDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
        if (interval.saleDay != 2) {
            std::cerr << "6.1 failure " << interval.saleDay << " != \n";
            std::cerr << "diff " << interval.priceDifference << "\n";
        }
    }
}
