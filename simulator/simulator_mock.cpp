#include <algorithm>
#include <array>
#include <cstdint>
#include <assert.h>
#include <memory>
#include <iostream>

#include "simulator_rpc.h"
#include "simulator_interface.h"

constexpr uint32_t framebuffer_width = 300; 
constexpr uint32_t framebuffer_height = 200; 
constexpr uint32_t pixel_bytes = 4;
constexpr uint32_t framebuffer_bytes = framebuffer_width * framebuffer_height * pixel_bytes;
constexpr uint32_t data_size = 256*1024*1024;

class MockSimulatorInterface : public SimulatorRPCInterface {
    //256MB of space
    std::array<uint8_t,data_size> data;
public:
    MockSimulatorInterface() {
        std::fill(data.begin(), data.begin()+data_size, 0);
        for (int i = 0; i < data.size(); i+=4){
            data[i+3] = 0xFF; //WHITE
            data[i+2] = 0xFF;
            data[i+1] = 0xFF;
            data[i] = 0xFF; //OPAQUE
        }
    }

    virtual std::array<uint8_t,512> get_memory_segment(uint32_t addr) {
        assert((addr & 0xFF) == 0);
        std::array<uint8_t,512> ret;
        std::copy(data.begin()+addr,data.begin()+addr+512, ret.begin());
        return ret;
    }
};

int main() {
    std::unique_ptr<SimulatorRPCInterface> interface = std::make_unique<MockSimulatorInterface>();
    ServerWrapper wrapper(true,interface);
    while (true) {
        std::cout << "Type 'exit' to end program cleanly" << std::endl;
        std::string inp;
        std::cin >> inp;

        if (inp == "exit") break;
    }
}

