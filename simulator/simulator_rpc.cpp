#include <grpcpp/server_builder.h>
#include <iostream>
#include <string>
#include <string_view>
#include <memory>

#include "sim_control.grpc.pb.h"

#include "simulator_rpc.h"

using grpc::Channel;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerBuilder;
using grpc::Server;

Status SimulatorServerImpl::SendCommand(
            ServerContext* context,
            const Command* command,
            CommandResponse* resp
) {
    std::cout << "Got command in server" << std::endl;
    resp->set_ok(true);
    return Status::OK;
}

void run_server() {
    std::string server_address("0.0.0.0:50101");
    SimulatorServerImpl service{};

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

