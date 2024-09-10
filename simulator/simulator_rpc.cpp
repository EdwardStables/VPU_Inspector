#include <iostream>
#include <string>
#include <string_view>
#include <memory>

#include "sim_control.grpc.pb.h"

#include "simulator_rpc.h"
#include "simulator_interface.h"

using grpc::Channel;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerBuilder;
using grpc::Server;

SimulatorServerImpl::SimulatorServerImpl(std::unique_ptr<SimulatorRPCInterface>& interface)
    :   simulator_interface(interface)
{
}

Status SimulatorServerImpl::SendCommand(
            ServerContext* context,
            const Command* command,
            CommandResponse* resp
) {
    std::cout << "Got command in server" << std::endl;
    resp->set_ok(true);
    return Status::OK;
}

Status SimulatorServerImpl::GetFramebuffer(
        ServerContext* context,
        const FramebufferRequest* command,
        ServerWriter<FramebufferSegment>* writer
) {
    FramebufferSegment fbs;
    for (int i = 0; i < 200*300; i++){
        fbs.add_data(0xFF0000FF); //Bright red
        if ((i+1)%100 == 0){
            writer->Write(fbs);
            fbs = FramebufferSegment();
        }
    }

    return Status::OK;
}

ServerWrapper::ServerWrapper(std::unique_ptr<SimulatorRPCInterface>& interface)
    : service(interface)
{
    run_server();
}

void ServerWrapper::run_server() {
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server = std::unique_ptr<Server>(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    auto r = [&]() {
        server->Wait();
    };
    server_thread = std::thread{r};
}

ServerWrapper::~ServerWrapper() {
    server->Shutdown();
    server_thread.join();
}

