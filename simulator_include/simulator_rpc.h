#pragma once
#include <string_view>
#include <thread>
#include <grpcpp/server_builder.h>

#include "sim_control.grpc.pb.h"

#include "simulator_interface.h"

using grpc::Channel;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;

class SimulatorServerImpl final : public SimInspector::Service {
    SimulatorRPCInterface* simulator_interface;
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
    SimulatorServerImpl(SimulatorRPCInterface* interface);
};

class ServerWrapper {
    bool enable = true;
    bool server_running = false;
    std::string server_address = "0.0.0.0:50101";
    std::unique_ptr<grpc::Server> server;
    std::thread server_thread;
    SimulatorRPCInterface* interface;

    void run_server();
public:
    bool is_server_running();
    ServerWrapper(bool enable, SimulatorRPCInterface* interface);
    ~ServerWrapper();
};

