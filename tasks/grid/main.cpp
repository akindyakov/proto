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
        //std::cerr << x << ":" << xSize << " " << y << ":" << ySize << '\n';
        return grid.at(y * xSize + x);
    }

    std::size_t xSize;
    std::size_t ySize;
    std::vector<int> grid;
};

int Count(const TGrid& grid) {
    auto prev = std::vector<int>(grid.xSize, 0);
    auto count = int{0};
    auto uid = int{0};
    for (auto y = std::size_t{0}; y < grid.ySize; ++y) {
        for (auto x = std::size_t{0}; x < grid.xSize; ++x) {
            if (grid.at(x, y)) {
                auto currUid = prev.at(x);
                if (0 == currUid) {
                    currUid = ++uid;
                    ++count;
                    prev.at(x) = currUid;
                }
                while (++x < grid.xSize && 0 != grid.at(x, y)) {
                    if (prev.at(x) != currUid) {
                        if (prev.at(x) == 0) {
                            prev.at(x) = currUid;
                        } else {
                            --count;
                            auto oldUid = prev.at(x);
                            for (auto i = x; i < grid.xSize; ++i) {
                                if (oldUid == prev.at(i)) {
                                    prev.at(i) = currUid;
                                }
                            }
                        }
                    }
                }
                --x;  // first zero(0) after non-zero sequence - if it is not out of range -> mark value to zero
            } else {
                prev.at(x) = 0;
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
        auto ans = Count(grid);
        if (ans != 3) {
            std::cerr << "0: failed " << ans << " != 3" << '\n';
            throw std::exception();
        } else {
            std::cerr << "0: success\n";
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
        grid.ySize = 4;
        auto ans = Count(grid);
        if (ans != 1) {
            std::cerr << "1: failed " << ans << " != 1" << '\n';
            throw std::exception();
        } else {
            std::cerr << "1: success\n";
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
        auto ans = Count(grid);
        if (ans != 10) {
            std::cerr << "2: failed " << ans << " != 10" << '\n';
            throw std::exception();
        } else {
            std::cerr << "2: success\n";
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
        auto ans = Count(grid);
        if (ans != 2) {
            std::cerr << "3: failed " << ans << " != 2" << '\n';
            throw std::exception();
        } else {
            std::cerr << "3: success\n";
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
        auto ans = Count(grid);
        if (ans != 1) {
            std::cerr << "4: failed " << ans << " != 2" << '\n';
            throw std::exception();
        } else {
            std::cerr << "4: success\n";
        }
    }
    {
        auto grid = TGrid(0, 0);
        grid.grid = std::vector<int>{
            1,0,0,0,1,
            1,1,1,0,1,
            0,0,1,0,1,
            1,0,1,0,1,
            1,0,1,0,1,
            1,1,1,1,1,
        };
        grid.xSize = 5;
        grid.ySize = 6;
        auto ans = Count(grid);
        if (ans != 1) {
            std::cerr << "5: failed " << ans << " != 1" << '\n';
            throw std::exception();
        } else {
            std::cerr << "5: success\n";
        }
    }
    {
        auto grid = TGrid(0, 0);
        grid.grid = std::vector<int>{
            1,0,0,0,1,0,0,0,0,1,
            1,1,1,0,1,0,1,1,0,1,
            0,0,1,0,1,0,0,1,0,1,
            1,0,1,0,1,0,0,1,0,1,
            1,0,1,0,1,0,0,1,0,1,
            1,1,1,1,1,0,1,1,1,1,
            0,0,0,0,1,0,0,0,0,1,
            1,1,1,0,1,1,1,1,0,1,
            0,0,1,0,0,0,0,1,0,1,
            1,0,1,0,1,0,0,1,0,1,
            1,0,1,0,0,1,0,1,0,1,
            1,1,1,0,0,0,0,1,1,1,
        };
        grid.xSize = 10;
        grid.ySize = 12;
        auto ans = Count(grid);
        if (ans != 4) {
            std::cerr << "5: failed " << ans << " != 4" << '\n';
            throw std::exception();
        } else {
            std::cerr << "5: success\n";
        }
    }
}
