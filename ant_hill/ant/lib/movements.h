#pragma once

#include "task.h"


namespace Ant {

class LookAroundTask
    : public TaskDependences
{
    void run(AntState& state) const override;
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

}  // namespace Ant
