#pragma once

#include "ant.h"


#include <unordered_map>
#include <unordered_set>


namespace Ant {

template<
    typename IteratorType
>
class IterRange {
public:
    explicit IterRange(
        IteratorType begin
        , IteratorType end
    )
        : begin_(begin)
        , end_(end)
    {
    }

    IteratorType begin() const {
        return begin_;
    }
    IteratorType cbegin() const {
        return begin_;
    }
    IteratorType end() const {
        return end_;
    }
    IteratorType cend() const {
        return end_;
    }
    IteratorType& begin() {
        return begin_;
    }
    IteratorType& end() {
        return end_;
    }

private:
    IteratorType begin_;
    IteratorType end_;
};

template<
    typename IteratorType
>
auto makeIterRange(
    IteratorType begin
    , IteratorType end
)
{
    return IterRange<IteratorType>(
        std::move(begin), std::move(end)
    );
}

template<
    typename TContainer
>
auto makeIterRange(
    TContainer& cont
)
{
    return IterRange<decltype(cont.begin())>(
        cont.begin(), cont.end()
    );
}

template<typename TContainer>
auto makeIterRange(
    const TContainer& cont
)
{
    return IterRange<decltype(cont.cbegin())>(
        cont.begin(), cont.end()
    );
}

class ITask;
//using TaskId = long int;

class TaskId
{
public:
    using Type = long int;
    Type id;

private:
    static constexpr auto InvalidId_ = Type{0};

public:
    explicit constexpr TaskId(
        Type id_
    ) noexcept
        : id(id_)
    {
    }

    constexpr bool isValid() const noexcept {
        return id != InvalidId_;
    }

    constexpr bool isShared() const noexcept {
        return id > 0;
    }

    static constexpr const TaskId Invalid() noexcept {
        return TaskId(InvalidId_);
    }
};

std::ostream& operator<<(std::ostream& os, const TaskId& id);
std::istream& operator>>(std::istream& is, TaskId& id);

class TaskIdHash {
public:
    auto operator()(const TaskId& id) const {
        return std::hash<TaskId::Type>{}(id.id);
    }
};

inline bool operator == (
    const TaskId& first
    , const TaskId& second
) {
    return (
        first.id == second.id
    );
}

inline bool operator != (
    const TaskId& first
    , const TaskId& second
) {
    return (
        first.id != second.id
    );
}

class ITask
{
public:
    virtual void run() const = 0;

    using DependencesList = std::vector<TaskId>;
    virtual const DependencesList& dependences() const = 0;

    using DepIterRangeType = IterRange<DependencesList::const_iterator>;
    virtual DepIterRangeType dependOn() const = 0;

    virtual ~ITask() = default;
};

struct TaskDependences
{
    ITask::DependencesList dependences_;
};

using TaskLibrary = std::unordered_map<
    TaskId,
    std::unique_ptr<ITask>,
    TaskIdHash
>;

class EmptyTask
    : public ITask
    , public TaskDependences
{
public:
    explicit EmptyTask() = default;
    EmptyTask(const EmptyTask&) = default;
    EmptyTask(EmptyTask&&) = default;

    void run() const override {
    }

    const ITask::DependencesList& dependences() const override {
        return dependences_;
    }

    ITask::DepIterRangeType dependOn() const override {
        return makeIterRange(dependences_);
    }
};

// class LookAround
//     : public ITask
// {
// };
// 
// class BearTask
//     : public ITask
// {
// public:
//     Map::EMaterial what;
//     Map::Point toWhere;
// };
// 
// class ConstructionTask
//     : public ITask
// {
// public:
//     explicit ConstructionTask(
//         Map::Field<Map::EMaterial> plan
//     );
// 
//     BearTask next();
// 
// private:
//     const FieldInMemory& fieldRef;
// };

/**
*   struct CompletedTask {
*       Map::ObjectId WorkerId;
*   };
*
*   struct WaitingDependsTask {
*       // who wants to know ?
*       std::unordered_set<Map::ObjectId> subscribers;
*   };
*
*   struct WaitingRunTask {
*       // who wants to know ?
*       std::unordered_set<Map::ObjectId> subscribers;
*   };
*
*   struct InProgressTask {
*       Map::ObjectId WorkerId;
*       // who wants to know ?
*       std::unordered_set<Map::ObjectId> subscribers;
*   };
*/

class CompletedTask
{
public:
    explicit CompletedTask(
        Map::ObjectId whoDoIt
    )
        : workerId(whoDoIt)
    {
    }

    Map::ObjectId workerId;
    int time;
};

class TaskInQueue {
public:
    explicit TaskInQueue(
        Map::ObjectId publisherId
    )
        : publisherId_(publisherId)
        , workerId_(Map::ObjectId::Invalid())
    {
    }

    void resetWorker(
        Map::ObjectId id = Map::ObjectId::Invalid()
    ) {
        workerId_ = id;
    }

    bool inProgress() const {
        return workerId_.isValid();
    }
    bool isReady() const {
        return dependences.empty();
    }

    void satisfy(TaskId id) {
        dependences.erase(id);
    }

    void subscribe(TaskId id) {
        subscribers.insert(id);
    }

private:
    const Map::ObjectId publisherId_ = Map::ObjectId::Invalid();
    Map::ObjectId workerId_ = Map::ObjectId::Invalid();

public:
    std::unordered_set<TaskId, TaskIdHash> dependences;
    std::unordered_set<TaskId, TaskIdHash> subscribers;
};

class TaskEmploymentService
{
public:
    explicit TaskEmploymentService(
        const TaskLibrary& taskLibrary
    )
        : taskLibrary_(taskLibrary)
    {
    }

    class Error
        : public Exception
    {
    };

    TaskInQueue& add(
        Map::ObjectId autorId
        , TaskId taskId
    ) {
        auto taskIter = tasks.find(taskId);
        if (taskIter == tasks.end()) {
            taskIter = tasks.emplace(
                taskId, TaskInQueue(autorId)
            ).first;
            for (const auto& dep : taskLibrary_.at(taskId)->dependences()) {
                if (!completedTasks.count(dep)) {
                    taskIter->second.dependences.insert(dep);
                    auto depTask = add(autorId, dep);
                    depTask.subscribe(taskId);
                }
            }
            if (taskIter->second.isReady()) {
                tasksToGo.push_back(taskId);
            }
        }
        return taskIter->second;
    }

    TaskId obtain(
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

    void complete(
        Map::ObjectId id
        , TaskId taskId
    ) {
        auto taskIter = tasks.find(taskId);
        if (taskIter != tasks.end()) {
            completedTasks.emplace(
                taskId, CompletedTask(id)
            );
            for (const auto& subId : taskIter->second.subscribers) {
                auto& task = tasks.at(subId);
                task.satisfy(taskId);
                if (task.isReady()) {
                    tasksToGo.push_back(subId);
                }
            }
            tasks.erase(taskIter);
        }
    }

private:
    const TaskLibrary& taskLibrary_;

    // dependences waiting tasks and ready-to-go tasks
    std::unordered_map<TaskId, TaskInQueue, TaskIdHash> tasks;

    // queue to run (only ready-to-go tasks here (only id))
    std::deque<TaskId> tasksToGo;

    // set of completed task to understanding what dependency is satisfied
    std::unordered_map<TaskId, CompletedTask, TaskIdHash> completedTasks;
};

}  // namespace Ant
