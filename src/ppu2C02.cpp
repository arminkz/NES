#include "ppu2C02.h"

#include "Renderer.h"

PPU2C02::PPU2C02()
{
	//Initialize PAL Screen colors
	palScreen[0x00] = glm::vec3(84, 84, 84 );
	palScreen[0x01] = glm::vec3(0, 30, 116 );
	palScreen[0x02] = glm::vec3(8, 16, 144 );
	palScreen[0x03] = glm::vec3(48, 0, 136 );
	palScreen[0x04] = glm::vec3(68, 0, 100 );
	palScreen[0x05] = glm::vec3(92, 0, 48 );
	palScreen[0x06] = glm::vec3(84, 4, 0 );
	palScreen[0x07] = glm::vec3(60, 24, 0 );
	palScreen[0x08] = glm::vec3(32, 42, 0 );
	palScreen[0x09] = glm::vec3(8, 58, 0 );
	palScreen[0x0A] = glm::vec3(0, 64, 0 );
	palScreen[0x0B] = glm::vec3(0, 60, 0 );
	palScreen[0x0C] = glm::vec3(0, 50, 60 );
	palScreen[0x0D] = glm::vec3(0, 0, 0 );
	palScreen[0x0E] = glm::vec3(0, 0, 0 );
	palScreen[0x0F] = glm::vec3(0, 0, 0 );

	palScreen[0x10] = glm::vec3(152, 150, 152 );
	palScreen[0x11] = glm::vec3(8, 76, 196 );
	palScreen[0x12] = glm::vec3(48, 50, 236 );
	palScreen[0x13] = glm::vec3(92, 30, 228 );
	palScreen[0x14] = glm::vec3(136, 20, 176 );
	palScreen[0x15] = glm::vec3(160, 20, 100 );
	palScreen[0x16] = glm::vec3(152, 34, 32 );
	palScreen[0x17] = glm::vec3(120, 60, 0 );
	palScreen[0x18] = glm::vec3(84, 90, 0 );
	palScreen[0x19] = glm::vec3(40, 114, 0 );
	palScreen[0x1A] = glm::vec3(8, 124, 0 );
	palScreen[0x1B] = glm::vec3(0, 118, 40 );
	palScreen[0x1C] = glm::vec3(0, 102, 120 );
	palScreen[0x1D] = glm::vec3(0, 0, 0 );
	palScreen[0x1E] = glm::vec3(0, 0, 0 );
	palScreen[0x1F] = glm::vec3(0, 0, 0 );

	palScreen[0x20] = glm::vec3(236, 238, 236 );
	palScreen[0x21] = glm::vec3(76, 154, 236 );
	palScreen[0x22] = glm::vec3(120, 124, 236 );
	palScreen[0x23] = glm::vec3(176, 98, 236 );
	palScreen[0x24] = glm::vec3(228, 84, 236 );
	palScreen[0x25] = glm::vec3(236, 88, 180 );
	palScreen[0x26] = glm::vec3(236, 106, 100 );
	palScreen[0x27] = glm::vec3(212, 136, 32 );
	palScreen[0x28] = glm::vec3(160, 170, 0 );
	palScreen[0x29] = glm::vec3(116, 196, 0 );
	palScreen[0x2A] = glm::vec3(76, 208, 32 );
	palScreen[0x2B] = glm::vec3(56, 204, 108 );
	palScreen[0x2C] = glm::vec3(56, 180, 204 );
	palScreen[0x2D] = glm::vec3(60, 60, 60 );
	palScreen[0x2E] = glm::vec3(0, 0, 0 );
	palScreen[0x2F] = glm::vec3(0, 0, 0 );

	palScreen[0x30] = glm::vec3(236, 238, 236 );
	palScreen[0x31] = glm::vec3(168, 204, 236 );
	palScreen[0x32] = glm::vec3(188, 188, 236 );
	palScreen[0x33] = glm::vec3(212, 178, 236 );
	palScreen[0x34] = glm::vec3(236, 174, 236 );
	palScreen[0x35] = glm::vec3(236, 174, 212 );
	palScreen[0x36] = glm::vec3(236, 180, 176 );
	palScreen[0x37] = glm::vec3(228, 196, 144 );
	palScreen[0x38] = glm::vec3(204, 210, 120 );
	palScreen[0x39] = glm::vec3(180, 222, 120 );
	palScreen[0x3A] = glm::vec3(168, 226, 144 );
	palScreen[0x3B] = glm::vec3(152, 226, 180 );
	palScreen[0x3C] = glm::vec3(160, 214, 228 );
	palScreen[0x3D] = glm::vec3(160, 162, 160 );
	palScreen[0x3E] = glm::vec3(0, 0, 0 );
	palScreen[0x3F] = glm::vec3(0, 0, 0 );

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
