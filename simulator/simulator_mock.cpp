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
    std::unique_ptr<std::array<uint8_t,data_size>>& data;
public:
    MockSimulatorInterface(std::unique_ptr<std::array<uint8_t,data_size>>& data) : data(data) {
        for (int i = 0; i < data->size(); i+=4){
            data->at(i+3) = 0xFF; //WHITE
            data->at(i+2) = 0xFF;
            data->at(i+1) = 0xFF;
            data->at(i) = 0xFF; //OPAQUE
        }
    }

    virtual std::array<uint8_t,512> get_memory_segment(uint32_t addr) {
        assert((addr & 0xFF) == 0);
        std::array<uint8_t,512> ret;
        std::copy(data->begin()+addr,data->begin()+addr+512, ret.begin());
        return ret;
    }

    virtual std::vector<std::string> get_source_code() {
        return {
            "this is a test input",
            "", 
            "", 
            "", 
            "this is the last line"
        };
    }

    void set_command(CommandType command) {}

    uint32_t get_pc() {
        return 0;
    }
};

int main() {
    //256MB of space
    auto data = std::make_unique<std::array<uint8_t,data_size>>();
    std::fill(data->begin(), data->begin()+data_size, 0);
    std::unique_ptr<SimulatorRPCInterface> interface = std::make_unique<MockSimulatorInterface>(data);
    ServerWrapper wrapper(true,interface.get());
    while (true) {
        std::cout << "Type 'exit' to end program cleanly" << std::endl;
        std::string inp;
        std::cin >> inp;

        if (inp == "exit") break;
    }
}

