#include <iostream>
#include <vector>

using Grid = std::vector<
  std::vector<int>
>;

void seeGlobe(
  const Grid& globe
  , Grid& seen
  , int uid
  , int x
  , int y
) {
  if (
    x < 0 || x >= static_cast<int>(seen[0].size())
    || y < 0 || y >= static_cast<int>(seen.size())
  ) {
    return;
  }

  if (seen[y][x] != 0) {
    return;
  }
  if (0 == globe[y][x]) {
    seen[y][x] = -1;
    return;
  }
  seen[y][x] = uid;
  seeGlobe(globe, seen, uid, x + 1, y);
  seeGlobe(globe, seen, uid, x - 1, y);
  seeGlobe(globe, seen, uid, x, y + 1);
  seeGlobe(globe, seen, uid, x, y - 1);
}

int countIslands(const Grid& globe) {
  auto uid = int{0};
  if (globe.empty()) {
    return uid;
  }
  auto seen = globe;
  for (auto y = size_t{0}; y < seen.size(); ++y) {
    for (auto x = size_t{0}; x < seen[0].size(); ++x) {
      seen[y][x] = 0;
    }
  }
  for (auto y = size_t{0}; y < seen.size(); ++y) {
    for (auto x = size_t{0}; x < seen[0].size(); ++x) {
      if (0 != globe[y][x] && 0 == seen[y][x]) {
        ++uid;
        seeGlobe(
          globe, seen, uid,
          static_cast<int>(x),
          static_cast<int>(y)
        );
      }
    }
  }
  return uid;
}

int main() {
  auto globe = Grid{{ 0,1,1 },
                      { 0,0,1 },
                      { 0,1,0 }};
  std::cout << countIslands(globe) << '\n';
  auto globe2 = Grid{{ 0,1,1,0,0,0,0,1,1,0 },
                      { 0,0,1,0,0,0,0,0,0,0 },
                      { 0,0,0,1,0,0,0,0,0,0 },
                      { 0,0,0,0,0,0,0,0,0,0 },
                      { 0,0,0,0,0,0,1,0,0,0 },
                      { 0,0,0,0,1,0,1,0,0,0 },
                      { 0,0,0,0,1,1,1,0,0,0 },
                      { 0,1,0,0,0,0,0,0,0,0 },
                      { 1,1,1,0,0,0,0,0,0,0 },
                      { 0,1,0,0,0,0,0,0,1,0 }};
  std::cout << countIslands(globe2) << '\n';
}
