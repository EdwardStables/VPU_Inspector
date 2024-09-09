#pragma once
#include <string_view>
#include "sim_control.grpc.pb.h"

using grpc::Channel;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerBuilder;
using grpc::Server;

class SimulatorServerImpl final : public SimInspector::Service {
    Status SendCommand(
            ServerContext* context,
            const Command* command,
            CommandResponse* resp
    ) override;
};

void run_server();

