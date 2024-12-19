#include "ppu2C02.h"

#include "Renderer.h"

PPU2C02::PPU2C02()
{
	//Initialize PAL Screen colors
	palScreen[0x00] = glm::vec4(84, 84, 84, 255);
	palScreen[0x01] = glm::vec4(0, 30, 116, 255);
	palScreen[0x02] = glm::vec4(8, 16, 144, 255);
	palScreen[0x03] = glm::vec4(48, 0, 136, 255);
	palScreen[0x04] = glm::vec4(68, 0, 100, 255);
	palScreen[0x05] = glm::vec4(92, 0, 48, 255);
	palScreen[0x06] = glm::vec4(84, 4, 0, 255);
	palScreen[0x07] = glm::vec4(60, 24, 0, 255);
	palScreen[0x08] = glm::vec4(32, 42, 0, 255);
	palScreen[0x09] = glm::vec4(8, 58, 0, 255);
	palScreen[0x0A] = glm::vec4(0, 64, 0, 255);
	palScreen[0x0B] = glm::vec4(0, 60, 0, 255);
	palScreen[0x0C] = glm::vec4(0, 50, 60, 255);
	palScreen[0x0D] = glm::vec4(0, 0, 0, 255);
	palScreen[0x0E] = glm::vec4(0, 0, 0, 255);
	palScreen[0x0F] = glm::vec4(0, 0, 0, 255);

	palScreen[0x10] = glm::vec4(152, 150, 152, 255);
	palScreen[0x11] = glm::vec4(8, 76, 196, 255);
	palScreen[0x12] = glm::vec4(48, 50, 236, 255);
	palScreen[0x13] = glm::vec4(92, 30, 228, 255);
	palScreen[0x14] = glm::vec4(136, 20, 176, 255);
	palScreen[0x15] = glm::vec4(160, 20, 100, 255);
	palScreen[0x16] = glm::vec4(152, 34, 32, 255);
	palScreen[0x17] = glm::vec4(120, 60, 0, 255);
	palScreen[0x18] = glm::vec4(84, 90, 0, 255);
	palScreen[0x19] = glm::vec4(40, 114, 0, 255);
	palScreen[0x1A] = glm::vec4(8, 124, 0, 255);
	palScreen[0x1B] = glm::vec4(0, 118, 40, 255);
	palScreen[0x1C] = glm::vec4(0, 102, 120, 255);
	palScreen[0x1D] = glm::vec4(0, 0, 0, 255);
	palScreen[0x1E] = glm::vec4(0, 0, 0, 255);
	palScreen[0x1F] = glm::vec4(0, 0, 0, 255);

	palScreen[0x20] = glm::vec4(236, 238, 236, 255);
	palScreen[0x21] = glm::vec4(76, 154, 236, 255);
	palScreen[0x22] = glm::vec4(120, 124, 236, 255);
	palScreen[0x23] = glm::vec4(176, 98, 236, 255);
	palScreen[0x24] = glm::vec4(228, 84, 236, 255);
	palScreen[0x25] = glm::vec4(236, 88, 180, 255);
	palScreen[0x26] = glm::vec4(236, 106, 100, 255);
	palScreen[0x27] = glm::vec4(212, 136, 32, 255);
	palScreen[0x28] = glm::vec4(160, 170, 0, 255);
	palScreen[0x29] = glm::vec4(116, 196, 0, 255);
	palScreen[0x2A] = glm::vec4(76, 208, 32, 255);
	palScreen[0x2B] = glm::vec4(56, 204, 108, 255);
	palScreen[0x2C] = glm::vec4(56, 180, 204, 255);
	palScreen[0x2D] = glm::vec4(60, 60, 60, 255);
	palScreen[0x2E] = glm::vec4(0, 0, 0, 255);
	palScreen[0x2F] = glm::vec4(0, 0, 0, 255);

	palScreen[0x30] = glm::vec4(236, 238, 236, 255);
	palScreen[0x31] = glm::vec4(168, 204, 236, 255);
	palScreen[0x32] = glm::vec4(188, 188, 236, 255);
	palScreen[0x33] = glm::vec4(212, 178, 236, 255);
	palScreen[0x34] = glm::vec4(236, 174, 236, 255);
	palScreen[0x35] = glm::vec4(236, 174, 212, 255);
	palScreen[0x36] = glm::vec4(236, 180, 176, 255);
	palScreen[0x37] = glm::vec4(228, 196, 144, 255);
	palScreen[0x38] = glm::vec4(204, 210, 120, 255);
	palScreen[0x39] = glm::vec4(180, 222, 120, 255);
	palScreen[0x3A] = glm::vec4(168, 226, 144, 255);
	palScreen[0x3B] = glm::vec4(152, 226, 180, 255);
	palScreen[0x3C] = glm::vec4(160, 214, 228, 255);
	palScreen[0x3D] = glm::vec4(160, 162, 160, 255);
	palScreen[0x3E] = glm::vec4(0, 0, 0, 255);
	palScreen[0x3F] = glm::vec4(0, 0, 0, 255);

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
	// Fake some sort of rendering process for now
	Renderer::getInstance()->draw_pixel(cycle-1, scanline, palScreen[(rand() % 2) ? 0x3F : 0x30]);
	Renderer::getInstance()->refresh_screen();

    // Advance renderer
	cycle++;
	if (cycle >= 341)
	{
		cycle = 0;
		scanline++;
		if (scanline >= 261)
		{
			scanline = -1;
			frameComplete = true;
		}
	}

}
