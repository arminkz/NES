#pragma once

#include "stdafx.h"

class Mapper
{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();

public:
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
    uint8_t nPrgBanks = 0;
    uint8_t nChrBanks = 0;

};