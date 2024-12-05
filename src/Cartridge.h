#pragma once

#include "stdafx.h"

#include "Mapper.h"

class Cartridge {

public:
    Cartridge(const std::string& fileName);
    ~Cartridge();

private:
    //Devices (Memories) on the Cartridge
    std::vector<uint8_t> prgMemory;
    std::vector<uint8_t> chrMemory;

    //Mapper Information
    uint8_t mapperID = 0;
    uint8_t nPrgBanks = 0;
    uint8_t nChrBanks = 0;

    std::shared_ptr<Mapper> pMapper;

public:
    //Communications on the CPU Bus
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    //Communications on the PPU Bus
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);
};