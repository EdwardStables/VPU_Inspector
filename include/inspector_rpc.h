#pragma once
#include <algorithm>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <array>

#include "sim_control.grpc.pb.h"

class InspectorClient {
    std::unique_ptr<SimInspector::Stub> stub_;
public:
    InspectorClient(std::shared_ptr<grpc::Channel> channel);
    bool Pause();
    bool Run();
    bool Step();
    bool Stop();
    bool SendCommand(CommandType action);
    bool GetFrameBuffer(std::unique_ptr<std::array<uint32_t,200*300>>& data);
    bool GetSourceCode(std::vector<std::string>& lines);
    bool GetPC(uint32_t& pc);
};

