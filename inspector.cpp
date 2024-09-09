#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <iostream>

#include "sim_control.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class InspectorClient {
    std::unique_ptr<SimInspector::Stub> stub_;
public:
    InspectorClient(std::shared_ptr<Channel> channel)
        : stub_(SimInspector::NewStub(channel)) {}

    bool SendCommand() {
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
};

int main() {
    InspectorClient client(grpc::CreateChannel("localhost:50101", grpc::InsecureChannelCredentials()));
    bool ok = client.SendCommand();
    std::cout << "Server gave " << (ok ? "OK" : "not OK") << std::endl;

    return 0;
}
