#pragma once
#include <grpcpp/grpcpp.h>
#include <memory>

#include "sim_control.grpc.pb.h"

class InspectorClient {
    std::unique_ptr<SimInspector::Stub> stub_;
public:
    InspectorClient(std::shared_ptr<grpc::Channel> channel);
    bool SendCommand();
    bool GetFrameBuffer();
};

