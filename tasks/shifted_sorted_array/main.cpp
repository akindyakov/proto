#include <iostream>
#include <vector>

std::size_t lookForBegin(
    const std::vector<int>& arr
) {
    if (arr.size() < 2) {
        return 0;
    }
    auto left = std::size_t{0};
    auto right = arr.size() - 1;
    auto mid = right;
    while (left < right) {
        mid = (left + right + 1) / 2;
        if (arr[left] < arr[right]) {
            return left;
        } else {
            if (arr[mid] < arr[left]) {
                if (mid == right) {
                    break;
                }
                right = mid;
            } else {
                if (mid == left) {
                    break;
                }
                left = mid;
            }
        }
    }
    if (arr[right] < arr[left]) {
        return right;
    }
    return left;
}

int main() {
    {
        auto ans = lookForBegin({1, 1, 1, 0, 0, 0});
        if (ans != 3) {
            std::cerr << "0: failed\n";
            return 1;
        }
    }
    {
        auto ans = lookForBegin({1, 2, 3, 4});
        if (ans != 0) {
            std::cerr << "1: failed\n";
            return 1;
        }
    }
    {
        auto ans = lookForBegin({1, 2, 3, 4, 0, 0});
        if (ans != 4) {
            std::cerr << "2: failed\n";
            std::cerr << ans << " != 4\n";
            return 1;
        }
    }
    {
        auto ans = lookForBegin({0, 0, 0, 0, 0, 0});
        if (ans != 0) {
            std::cerr << "3: failed\n";
            std::cerr << ans << " != 0\n";
            return 1;
        }
    }
    return 0;
}

