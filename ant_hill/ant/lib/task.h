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

    virtual void setDependsOn(TaskId id) = 0;

    using DependencesList = std::unordered_set<TaskId, TaskIdHash>;
    virtual const DependencesList& dependences() const = 0;

    using DepIterRangeType = IterRange<DependencesList::const_iterator>;
    virtual DepIterRangeType dependOn() const = 0;

    virtual ~ITask() = default;
};

class TaskDependences
    : public ITask
{
public:
    ITask::DependencesList dependences_;

public:
    void setDependsOn(TaskId depId) override {
        this->dependences_.insert(
            std::move(depId)
        );
    }

    inline const ITask::DependencesList& dependences() const override {
        return dependences_;
    }

    inline ITask::DepIterRangeType dependOn() const override {
        return makeIterRange(dependences_);
    }
};

using TaskLibrary = std::unordered_map<
    TaskId,
    std::unique_ptr<ITask>,
    TaskIdHash
>;

class EmptyTask
    : public TaskDependences
{
public:
    explicit EmptyTask() = default;
    EmptyTask(const EmptyTask&) = default;
    EmptyTask(EmptyTask&&) = default;

    void run() const override {
    }
};

class LookAround
    : public TaskDependences
{
    void run() const override {
    }
};

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

    bool isBelongsTo(
        Map::ObjectId id
    ) {
        return this->workerId_ == id;
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
        //*dbg*/ std::cerr << "subscribers number is " << subscribers.size() << '\n';
    }

private:
    const Map::ObjectId publisherId_ = Map::ObjectId::Invalid();
    Map::ObjectId workerId_ = Map::ObjectId::Invalid();

public:
    std::unordered_set<TaskId, TaskIdHash> dependences;
    // could vector be here instead of set?
    std::unordered_set<TaskId, TaskIdHash> subscribers;
};

/**
* Static topological sort is more easy and no so large algorithm is terms of memory usage.
* But how should I use it in multithread, multiprocess and multiservers scheduling?
*/
class TaskEmploymentService
{
public:
    explicit TaskEmploymentService(
        const TaskLibrary& taskLibrary
    );

    class Error
        : public Exception
    {
    };

    void add(
        Map::ObjectId autorId
        , TaskId taskId
    );

    TaskId obtain(
        Map::ObjectId workerId
    );

    void complete(
        Map::ObjectId id
        , TaskId taskId
    );

private:
    TaskInQueue& addRec(
        Map::ObjectId autorId
        , TaskId taskId
    );

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
