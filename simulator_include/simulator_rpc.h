#pragma once
#include <string_view>
#include "sim_control.grpc.pb.h"

#include "simulator_interface.h"

using grpc::Channel;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;

class SimulatorServerImpl final : public SimInspector::Service {
    std::unique_ptr<SimulatorRPCInterface>& simulator_interface;
    Status SendCommand(
            ServerContext* context,
            const Command* command,
            CommandResponse* resp
    ) override;
    Status GetFramebuffer(
            ServerContext* context,
            const FramebufferRequest* command,
            ServerWriter<FramebufferSegment>* resp
    ) override;
public:
    SimulatorServerImpl(std::unique_ptr<SimulatorRPCInterface>& interface);
};

void run_server(std::unique_ptr<SimulatorRPCInterface>& interface);

