#pragma once

#include "stdafx.h"


enum NTMIRROR
{
	HARDWARE,
	HORIZONTAL,
	VERTICAL,
	ONESCREEN_LO,
	ONESCREEN_HI,
};


class Mapper
{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();

public:
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr, uint8_t &data) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) = 0;

    virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

    virtual void reset() = 0;

    virtual NTMIRROR getMirroringMode() = 0;

    //virtual bool irqState() = 0;
    //virtual void irqClear() = 0;
    
    //virtual void scanline() = 0;

protected:
    uint8_t nPrgBanks = 0;
    uint8_t nChrBanks = 0;

};