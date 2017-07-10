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

TaskEmploymentService::TaskEmploymentService(
    const TaskLibrary& taskLibrary
)
    : taskLibrary_(taskLibrary)
{
}

void TaskEmploymentService::add(
    Map::ObjectId autorId
    , TaskId taskId
) {
    this->addRec(
        std::move(autorId),
        std::move(taskId)
    );
}

TaskInQueue& TaskEmploymentService::addRec(
    Map::ObjectId autorId
    , TaskId taskId
) {
    //*dbg*/ std::cerr << "add: " << taskId << std::endl;
    auto taskIter = tasks.find(taskId);
    if (taskIter == tasks.end() && !completedTasks.count(taskId)) {
        taskIter = tasks.emplace(
            taskId, TaskInQueue(autorId)
        ).first;
        for (const auto& dep : taskLibrary_.at(taskId)->dependOn()) {
            //*dbg*/ std::cerr << "depend on: " << dep << '\n';
            if (!completedTasks.count(dep)) {
                taskIter->second.dependences.insert(dep);
                auto& depTask = addRec(autorId, dep);
                //*dbg*/ std::cerr << " - " << dep << " is not completed, so subscribe " << taskId << "\n";
                depTask.subscribe(taskId);
            }
        }
        if (taskIter->second.isReady()) {
            tasksToGo.push_back(taskId);
        }
    }
    return taskIter->second;
}

TaskId TaskEmploymentService::obtain(
    Map::ObjectId workerId
) {
    if (workerId.isValid() && !tasksToGo.empty()) {
        auto taskId = tasksToGo.front();
        tasksToGo.pop_front();
        auto& task = tasks.at(taskId);
        task.resetWorker(workerId);
        return taskId;
    }
    return Ant::TaskId{0};
}

void TaskEmploymentService::complete(
    Map::ObjectId id
    , TaskId taskId
) {
    auto taskIter = tasks.find(taskId);
    if (
        taskIter != tasks.end()
        && taskIter->second.inProgress()
        && taskIter->second.isBelongsTo(id)
    ) {
        //*dbg*/ std::cerr << "task (" << taskId << ") is exist, inProgress and so on\n";
        completedTasks.emplace(
            taskId, CompletedTask(id)
        );
        for (const auto& subId : taskIter->second.subscribers) {
            //*dbg*/ std::cerr << "subId: " << subId << '\n';
            auto& task = tasks.at(subId);
            task.satisfy(taskId);
            if (task.isReady()) {
                tasksToGo.push_back(subId);
            }
        }
        tasks.erase(taskIter);
    }
}

}  // namespace Ant
