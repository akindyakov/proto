#include <iostream>
#include <string>
#include <algorithm>


int firstDifferentPos(
    const std::string& first
    , const std::string& second
) {
    auto i = size_t{0};
    auto max = std::min(first.size(), second.size());
    while (
        i < max
        && first[i] == second[i]
    ) {
        ++i;
    }
    return i;
}

bool isPosible(
    const std::string& first
    , const std::string& second
    , int number
) {
    auto diffPos = firstDifferentPos(first, second);
    auto honesSteps = static_cast<int>(first.size() + second.size()) - 2 * diffPos;
    if (honesSteps == number) {
        return true;
    } else if (honesSteps < number) {
        auto reminder = number - honesSteps;
        if (reminder % 2) {
            if (diffPos < reminder) {
                return true;
            }
        } else {
            return true;
        }
    }
    return false;
}

int main() {
    auto first = std::string{};
    auto second = std::string{};
    auto number = int{0};
    std::cin >> first;
    std::cin >> second;
    std::cin >> number;
    if (isPosible(first, second, number)) {
        std::cout << "Yes";
    } else {
        std::cout << "No";
    }
}
