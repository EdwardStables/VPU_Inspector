#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <iostream>

#include "sim_control.grpc.pb.h"

#include "inspector_rpc.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

InspectorClient::InspectorClient(std::shared_ptr<Channel> channel)
    : stub_(SimInspector::NewStub(channel))
{}

bool InspectorClient::SendCommand() {
    Command command; 
    command.set_command(CommandType::PAUSE);
    command.set_step_size(0);
    
    CommandResponse response;
    ClientContext context;

    Status status = stub_->SendCommand(&context, command, &response);

    if (status.ok()){
        return response.ok();
    } else {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
    return false;
}


