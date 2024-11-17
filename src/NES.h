#pragma once
#include <cstdint>
#include <array>

//#include "cpu6502.h"

class NES {
public:
    NES();
    ~NES();

public: //Devices on the NES
    //CPU6502 cpu;
    
    //Fake RAM for now (64KB)
    std::array<uint8_t, 64 * 1024> ram;

public: //Bus Read/Write
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool readOnly = false);
};