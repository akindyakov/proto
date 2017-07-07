#include "task.h"

namespace Ant {

std::ostream& operator<<(std::ostream& os, const TaskId& id) {
    os << id.id;
    return os;
}

std::istream& operator>>(std::istream& is, TaskId& id) {
    is >> id.id;
    return is;
}

}  // namespace Ant
