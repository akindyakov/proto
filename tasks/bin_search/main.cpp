#include <iostream>
#include <vector>

bool less(int first, int second) {
    return first < second;
}

size_t lower(const std::vector<int>& arr, int value) {
    auto left = size_t{0};
    auto right = arr.size();
    while (true) {
        auto mid = (right + left) / 2;
        if (mid == left) {
            break;
        }
        if (less(arr[mid], value)) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

size_t upper(const std::vector<int>& arr, int value) {
    auto left = size_t{0};
    auto right = arr.size();
    while (true) {
        auto mid = (right + left) / 2;
        if (mid == left) {
            break;
        }
        if (less(value, arr[mid])) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return right;
}

int main() {
    std::cerr << lower({0, 1, 1, 2, 2, 2, 2, 3, 3}, 2) << std::endl;
    std::cerr << upper({0, 1, 1, 2, 2, 2, 2, 3, 3}, 2) << std::endl;
}
