#include "task.h"

#include <lib/tests/assert.h>

#include <iostream>

void createTest() {
    std::cerr << " - createTest\n";
    auto taskLib = Ant::TaskLibrary{};

    auto firstTaskId = Ant::TaskId{1};
    {
        auto task = std::make_unique<Ant::EmptyTask>();
        taskLib.emplace(firstTaskId, std::move(task));
    }
    auto secondTaskId = Ant::TaskId{2};
    {
        auto task = std::make_unique<Ant::EmptyTask>();
        task->setDependsOn(firstTaskId);
        taskLib.emplace(secondTaskId, std::move(task));
    }
    auto thirdTaskId = Ant::TaskId{3};
    {
        auto task = std::make_unique<Ant::EmptyTask>();
        task->setDependsOn(firstTaskId);
        taskLib.emplace(thirdTaskId, std::move(task));
    }
    auto fourthTaskId = Ant::TaskId{4};
    {
        auto task = std::make_unique<Ant::EmptyTask>();
        task->setDependsOn(secondTaskId);
        task->setDependsOn(thirdTaskId);
        task->setDependsOn(firstTaskId);
        taskLib.emplace(fourthTaskId, std::move(task));
    }

    auto objId = Map::ObjectId{2};
    auto otherObjId = Map::ObjectId{3};

    auto manager = Ant::TaskEmploymentService(taskLib);
    UT_ASSERT_EQUAL(Ant::TaskId::Invalid(), manager.obtain(objId));

    manager.add(objId, fourthTaskId);
    UT_ASSERT_EQUAL(firstTaskId, manager.obtain(objId));

    // first is not complete yet
    UT_ASSERT_EQUAL(Ant::TaskId::Invalid(), manager.obtain(otherObjId));
    manager.complete(objId, firstTaskId);
    {
        auto taskId = manager.obtain(objId);
        UT_ASSERT(taskId == secondTaskId || taskId == thirdTaskId);

        auto otherTaskId = manager.obtain(otherObjId);
        UT_ASSERT(otherTaskId == secondTaskId || otherTaskId == thirdTaskId);

        // second and third are not complete yet
        UT_ASSERT_EQUAL(Ant::TaskId::Invalid(), manager.obtain(objId));
        manager.complete(objId, taskId);
        manager.complete(otherObjId, otherTaskId);
    }
    {
        auto taskId = manager.obtain(objId);
        UT_ASSERT_EQUAL(taskId, fourthTaskId);
        manager.complete(objId, taskId);
    }
    UT_ASSERT_EQUAL(Ant::TaskId::Invalid(), manager.obtain(objId));
}

int main(int argn, char** argv) {
    try {
        std::cerr << "task_ut:\n";
        createTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
