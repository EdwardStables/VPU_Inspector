#pragma once
#include <cstdint>
#include <array>

class SimulatorRPCInterface {
public:
    //Get 4096 bits of memory
    virtual std::array<uint8_t,512> get_memory_segment(uint32_t address) = 0;
};
