#include "args.h"

#include <ant/lib/location.h>
#include <ant/lib/task.h>

#include <map/rpc/client.h>

#include <jsonrpccpp/client/connectors/httpclient.h>

#include <iostream>
#include <thread>
#include <chrono>

void runWorker(
    Map::JsonRPCClient& client
) {
    auto state = Ant::AntState(
        Ant::Location(
            Ant::LocationClient(client)
        )
    );
    auto taskLib = Ant::TaskLibrary{};
    auto firstTaskId = Ant::TaskId{1};
    taskLib.emplace(
        firstTaskId,
        std::make_unique<Ant::LookAroundTask>()
    );
    auto manager = Ant::TaskEmploymentService(taskLib);
    manager.add(state.location.id(), firstTaskId);

    while (true) {
        auto taskId = manager.obtain(state.location.id());
        if (!taskId.isValid()) {
            return;
        }
        auto& task = taskLib.at(taskId);
        task->run(state);
    }
}

int main(int argn, char** argv) {
    auto args = Argparse(argn, argv);
    if (!std::get<1>(args)) {
        return 1;
    }
    using namespace std::chrono_literals;
    auto httpclient = jsonrpc::HttpClient("http://localhost:8383");
    auto client = Map::JsonRPCClient{httpclient};
    runWorker(client);
}
