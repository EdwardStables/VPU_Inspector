#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/sync_stream.h>
#include <memory>
#include <iostream>

#include "sim_control.grpc.pb.h"

#include "inspector_rpc.h"
#include "sim_control.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

InspectorClient::InspectorClient(std::shared_ptr<Channel> channel)
    : stub_(SimInspector::NewStub(channel))
{}

bool InspectorClient::Pause() {
    return SendCommand(CommandType::PAUSE);
}

bool InspectorClient::Run() {
    return SendCommand(CommandType::RUN);
}

bool InspectorClient::Step() {
    return SendCommand(CommandType::STEP);
}

bool InspectorClient::Stop() {
    return SendCommand(CommandType::STOP);
}


bool InspectorClient::SendCommand(CommandType action) {
    Command command; 
    command.set_command(action);
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

bool InspectorClient::GetFrameBuffer(std::unique_ptr<std::array<uint32_t,200*300>>& data) {
    FramebufferRequest request; 
    ClientContext context;

    auto stream = stub_->GetFramebuffer(&context, request);

    int ind = 0;
    FramebufferSegment segment;
    while (stream->Read(&segment)) {
        for (auto& d : segment.data()) {
            assert(ind < 200*300);
            data->at(ind) = d;
            ind++;
        }
    }

    return true;
}

bool InspectorClient::GetSourceCode(std::vector<std::string>& lines) {
    SourceCodeRequest request;
    SourceCodeResponse response;
    ClientContext context;

    Status status = stub_->GetSourceCode(&context, request, &response);

    for (auto& line : response.lines()) {
        lines.push_back(line);
    }

    return true;
}