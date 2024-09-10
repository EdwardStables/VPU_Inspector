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
    uint32_t start_addr = 0;
    uint32_t end_addr = 200*300*4 + start_addr;

    int words = 0;
    FramebufferSegment fbs;
    for (int addr = start_addr; addr < end_addr; addr+=512){
        auto data = simulator_interface->get_memory_segment(addr);
        for (int d = 0; d < data.size(); d+=4) {
            if (addr+d >= end_addr) break; //framebuffer doesn't perfectly fit into 512
            uint32_t word = data[d+3] << 24;
            word |= data[d+2] << 16;
            word |= data[d+1] << 8;
            word |= data[d];
            fbs.add_data(word);
            std::cout << "word " << words++ << std::endl;
        }
        writer->Write(fbs);
        fbs = FramebufferSegment();
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

