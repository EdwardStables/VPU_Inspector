#pragma once
#include <cstdint>
#include <array>
#include <vector>

class SimulatorRPCInterface {
public:
    //Get 4096 bits of memory
    virtual std::array<uint8_t,512> get_memory_segment(uint32_t address) = 0;
    virtual std::vector<std::string> get_source_code() = 0;
};
