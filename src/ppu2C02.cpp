#include "ppu2C02.h"

PPU2C02::PPU2C02()
{
}

PPU2C02::~PPU2C02()
{
}

uint8_t PPU2C02::cpuRead(uint16_t addr, bool readOnly)
{
    return 0;
}

void PPU2C02::cpuWrite(uint16_t addr, uint8_t data)
{
}

uint8_t PPU2C02::ppuRead(uint16_t addr, bool readOnly)
{
    return 0;
}

void PPU2C02::ppuWrite(uint16_t addr, uint8_t data)
{
}

void PPU2C02::connectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cart = cartridge;
}

void PPU2C02::clock()
{
    
}
