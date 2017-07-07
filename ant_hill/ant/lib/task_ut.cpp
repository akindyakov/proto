#include "task.h"

#include <lib/tests/assert.h>

#include <iostream>

void createTest() {
    std::cerr << " - createTest\n";
    auto taskId = Ant::TaskId{1};
    auto taskLib = Ant::TaskLibrary{};

    taskLib.emplace(taskId, std::make_unique<Ant::EmptyTask>());

    auto manager = Ant::TaskEmploymentService(taskLib);
    UT_ASSERT_EQUAL(Ant::TaskId::Invalid(), manager.obtain(Map::ObjectId{2}));

    manager.add(Map::ObjectId{2}, taskId);
    {
        auto id = manager.obtain(Map::ObjectId{2});
        UT_ASSERT_EQUAL(taskId, id);
    }

    manager.complete(Map::ObjectId{2}, taskId);
    UT_ASSERT_EQUAL(Ant::TaskId::Invalid(), manager.obtain(Map::ObjectId{2}));
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
