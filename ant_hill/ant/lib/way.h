#pragma

namespace Ant {

class Way
{
public:
    std::unique_ptr<Map::RelativeDirection> next();
};

}  // namespace Ant
