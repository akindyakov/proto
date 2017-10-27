#include <iostream>
#include <vector>
#include <cstddef>

class TGrid {
public:
    explicit TGrid(
        std::size_t xSize_
        , std::size_t ySize_
    )
        : xSize(xSize_)
        , ySize(ySize_)
        , grid(xSize * ySize, 0)
    {
    }

    int at(
        std::size_t x
        , std::size_t y
    ) const {
        return grid[y * xSize + x];
    }

    std::size_t xSize;
    std::size_t ySize;
    std::vector<int> grid;
};

int count(const TGrid& grid) {
    auto prev = std::vector<int>(grid.xSize);
    auto counter = int{0};
    auto count = int{0};
    auto uid = int{0};
    for (auto x = std::size_t{0}; x < grid.xSize; ++x) {
        if (grid.at(x, 0)) {
            ++count;
            ++uid;
            while (grid.at(x, 0)) {
                prev[x] = uid;
                ++x;
            }
        }
    }
    for (auto y = std::size_t{1}; y < grid.ySize; ++y) {
        for (auto x = std::size_t{0}; x < grid.xSize; ++x) {
            if (grid.at(x, y)) {
                auto currUid = prev[x];
                if (0 == prev[x]) {
                    currUid = ++uid;
                    ++count;
                }
                while (x < grid.xSize && 0 != grid.at(x, y)) {
                    if (prev[x] != currUid) {
                        if (prev[x] == 0) {
                            prev[x] = currUid;
                        } else {
                            --count;
                            auto oldUid = prev[x];
                            for (auto i = x; i < grid.xSize; ++i) {
                                if (oldUid == prev[i]) {
                                    prev[i] = currUid;
                                }
                            }
                        }
                    }
                    ++x;
                }
                --x;
            } else {
                prev[x] = 0;
            }
        }
    }
    return count;
}

int main() {
    {
        auto grid = TGrid(0, 0);
        grid.grid = std::vector<int>{
            0,1,1,0,
            0,0,0,1,
            1,1,0,0,
            0,1,1,0,
        };
        grid.xSize = 4;
        grid.ySize = 4;
        auto ans = count(grid);
        if (ans != 3) {
            std::cerr << "0: failed " << ans << " != 3" << '\n';
            throw std::exception();
        }
    }
    {
        auto grid = TGrid(0, 0);
        grid.grid = std::vector<int>{
            1,0,0,0,1,
            1,0,1,0,1,
            1,1,1,0,1,
            0,0,1,1,1,
        };
        grid.xSize = 5;
        grid.ySize = 5;
        auto ans = count(grid);
        if (ans != 1) {
            std::cerr << "1: failed " << ans << " != 1" << '\n';
            throw std::exception();
        }
    }
    {
        auto grid = TGrid(0, 0);
        grid.grid = std::vector<int>{
            1,0,1,0,1,
            0,1,0,1,0,
            1,0,1,0,1,
            0,1,0,1,0,
        };
        grid.xSize = 5;
        grid.ySize = 4;
        auto ans = count(grid);
        if (ans != 10) {
            std::cerr << "2: failed " << ans << " != 10" << '\n';
            throw std::exception();
        }
    }
    {
        auto grid = TGrid(0, 0);
        grid.grid = std::vector<int>{
            0,1,1,1,
            0,1,0,1,
            0,1,0,0,
            0,1,0,1,
        };
        grid.xSize = 4;
        grid.ySize = 4;
        auto ans = count(grid);
        if (ans != 2) {
            std::cerr << "3: failed " << ans << " != 2" << '\n';
            throw std::exception();
        }
    }
    {
        auto grid = TGrid(0, 0);
        grid.grid = std::vector<int>{
            1,1,0,1,
            1,1,0,1,
            1,0,1,1,
            1,1,1,1,
        };
        grid.xSize = 4;
        grid.ySize = 4;
        auto ans = count(grid);
        if (ans != 1) {
            std::cerr << "4: failed " << ans << " != 2" << '\n';
            throw std::exception();
        }
    }
}
