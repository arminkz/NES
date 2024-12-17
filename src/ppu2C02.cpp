#include "ppu2C02.h"

PPU2C02::PPU2C02()
{
}

PPU2C02::~PPU2C02()
{
}

uint8_t PPU2C02::cpuRead(uint16_t addr, bool readOnly)
{
    //There are 8 addresses that can be read on the CPU Bus

    uint8_t data = 0x00;

	switch (addr)
	{
	case 0x0000: // Control
		break;
	case 0x0001: // Mask
		break;
	case 0x0002: // Status
		break;
	case 0x0003: // OAM Address
		break;
	case 0x0004: // OAM Data
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // PPU Address
		break;
	case 0x0007: // PPU Data
		break;
	}

	return data;
}

void PPU2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr)
	{
	case 0x0000: // Control
		break;
	case 0x0001: // Mask
		break;
	case 0x0002: // Status
		break;
	case 0x0003: // OAM Address
		break;
	case 0x0004: // OAM Data
		break;
	case 0x0005: // Scroll
		break;
	case 0x0006: // PPU Address
		break;
	case 0x0007: // PPU Data
		break;
	}
}

uint8_t PPU2C02::ppuRead(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (cart->ppuRead(addr, data))
	{
        //Cartridge has handled the read
	}

	return data;
}

void PPU2C02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF;

	if (cart->ppuWrite(addr, data))
	{
        //Cartridge has handled the write
	}
}

void PPU2C02::connectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cart = cartridge;
}

void PPU2C02::clock()
{
    
}
