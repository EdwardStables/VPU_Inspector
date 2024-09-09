#include "inspector_rpc.h"

int main() {
    InspectorClient client(grpc::CreateChannel("localhost:50101", grpc::InsecureChannelCredentials()));
    bool ok = client.SendCommand();
    std::cout << "Server gave " << (ok ? "OK" : "not OK") << std::endl;

    ok = client.GetFrameBuffer();
    std::cout << "Finished getting framebuffer" << std::endl;
    return 0;
}

