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

class ServerWrapper {
    bool enable = true;
    std::string server_address = "0.0.0.0:50101";
    SimulatorServerImpl service;
    grpc::ServerBuilder builder;
    std::unique_ptr<grpc::Server> server;

    std::thread server_thread;

    void run_server();
public:
    ServerWrapper(bool enable, std::unique_ptr<SimulatorRPCInterface>& interface);
    ~ServerWrapper();
};

