#pragma once

#include "stdafx.h"

#include "Cartridge.h"

class PPU2C02 {

public:
    PPU2C02();
    ~PPU2C02();

private:
    //Devices (Memories) on the PPU Bus
    uint8_t tblName[2][1024];    //2x 1KB Name Tables for Layout of Background Tiles
    uint8_t tblPalette[32];      //32x 8-bit RGB Color Palette
    uint8_t tblPattern[2][4096]; //2x 4KB Pattern Tables for Sprites and Backgrounds
 
public:
    //Communications on the CPU Bus
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    //Communications on the PPU Bus
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

public:
    void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();

    bool frameComplete = false;

    glm::vec3 palScreen[0x40]; // Color Palette

    int16_t scanline = 0;
    int16_t cycle = 0;

private:
    std::shared_ptr<Cartridge> cart;

};