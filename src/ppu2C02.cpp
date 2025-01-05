#include "PPU2C02.h"

#include "Renderer.h"
#include "NES.h"

PPU2C02::PPU2C02()
{
	//Initialize Palette
	for(int i = 0; i < 32; i++)
	{
		tblPalette[i] = 0;
	}

	//Initialize PAL Screen colors
	palColors[0x00] = glm::vec3(84, 84, 84);
	palColors[0x01] = glm::vec3(0, 30, 116);
	palColors[0x02] = glm::vec3(8, 16, 144);
	palColors[0x03] = glm::vec3(48, 0, 136);
	palColors[0x04] = glm::vec3(68, 0, 100);
	palColors[0x05] = glm::vec3(92, 0, 48);
	palColors[0x06] = glm::vec3(84, 4, 0);
	palColors[0x07] = glm::vec3(60, 24, 0);
	palColors[0x08] = glm::vec3(32, 42, 0);
	palColors[0x09] = glm::vec3(8, 58, 0);
	palColors[0x0A] = glm::vec3(0, 64, 0);
	palColors[0x0B] = glm::vec3(0, 60, 0);
	palColors[0x0C] = glm::vec3(0, 50, 60);
	palColors[0x0D] = glm::vec3(0, 0, 0);
	palColors[0x0E] = glm::vec3(0, 0, 0);
	palColors[0x0F] = glm::vec3(0, 0, 0);

	palColors[0x10] = glm::vec3(152, 150, 152);
	palColors[0x11] = glm::vec3(8, 76, 196);
	palColors[0x12] = glm::vec3(48, 50, 236);
	palColors[0x13] = glm::vec3(92, 30, 228);
	palColors[0x14] = glm::vec3(136, 20, 176);
	palColors[0x15] = glm::vec3(160, 20, 100);
	palColors[0x16] = glm::vec3(152, 34, 32);
	palColors[0x17] = glm::vec3(120, 60, 0);
	palColors[0x18] = glm::vec3(84, 90, 0);
	palColors[0x19] = glm::vec3(40, 114, 0);
	palColors[0x1A] = glm::vec3(8, 124, 0);
	palColors[0x1B] = glm::vec3(0, 118, 40);
	palColors[0x1C] = glm::vec3(0, 102, 120);
	palColors[0x1D] = glm::vec3(0, 0, 0);
	palColors[0x1E] = glm::vec3(0, 0, 0);
	palColors[0x1F] = glm::vec3(0, 0, 0);

	palColors[0x20] = glm::vec3(236, 238, 236);
	palColors[0x21] = glm::vec3(76, 154, 236);
	palColors[0x22] = glm::vec3(120, 124, 236);
	palColors[0x23] = glm::vec3(176, 98, 236);
	palColors[0x24] = glm::vec3(228, 84, 236);
	palColors[0x25] = glm::vec3(236, 88, 180);
	palColors[0x26] = glm::vec3(236, 106, 100);
	palColors[0x27] = glm::vec3(212, 136, 32);
	palColors[0x28] = glm::vec3(160, 170, 0);
	palColors[0x29] = glm::vec3(116, 196, 0);
	palColors[0x2A] = glm::vec3(76, 208, 32);
	palColors[0x2B] = glm::vec3(56, 204, 108);
	palColors[0x2C] = glm::vec3(56, 180, 204);
	palColors[0x2D] = glm::vec3(60, 60, 60);
	palColors[0x2E] = glm::vec3(0, 0, 0);
	palColors[0x2F] = glm::vec3(0, 0, 0);

	palColors[0x30] = glm::vec3(236, 238, 236);
	palColors[0x31] = glm::vec3(168, 204, 236);
	palColors[0x32] = glm::vec3(188, 188, 236);
	palColors[0x33] = glm::vec3(212, 178, 236);
	palColors[0x34] = glm::vec3(236, 174, 236);
	palColors[0x35] = glm::vec3(236, 174, 212);
	palColors[0x36] = glm::vec3(236, 180, 176);
	palColors[0x37] = glm::vec3(228, 196, 144);
	palColors[0x38] = glm::vec3(204, 210, 120);
	palColors[0x39] = glm::vec3(180, 222, 120);
	palColors[0x3A] = glm::vec3(168, 226, 144);
	palColors[0x3B] = glm::vec3(152, 226, 180);
	palColors[0x3C] = glm::vec3(160, 214, 228);
	palColors[0x3D] = glm::vec3(160, 162, 160);
	palColors[0x3E] = glm::vec3(0, 0, 0);
	palColors[0x3F] = glm::vec3(0, 0, 0);

	for(int i=0; i<64; i++)
	{
		auto color = palColors[i];
		palColors[i] = glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
	}

}

PPU2C02::~PPU2C02()
{
}

uint8_t PPU2C02::cpuRead(uint16_t addr, bool readOnly)
{
    //There are 8 addresses that can be read on the CPU Bus
	uint8_t data = 0x00;
	addr &= 0x0007;

	if (readOnly) {
		switch (addr)
		{
		case 0x0000: // Control;
			return control.reg;
		case 0x0001: // Mask
			return mask.reg;
		case 0x0002: // Status
			return (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
		case 0x0003: // OAM Address
			break;
		case 0x0004: // OAM Data
			data = pOAM[oam_addr];
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			break;
		}
	}
	else{
		switch (addr)
		{
		case 0x0000: // Control
			// Not readable
			break;
		case 0x0001: // Mask
			// Not readable
			break;
		case 0x0002: // Status
			data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
			status.vertical_blank = 0; // Clear vertical blank flag
			addr_latch = 0; // Reset Loopy Address latch
			break;
		case 0x0003: // OAM Address
			// Not readable
			break;
		case 0x0004: // OAM Data
			data = pOAM[oam_addr];
			break;
		case 0x0005: // Scroll
			break;
		case 0x0006: // PPU Address
			break;
		case 0x0007: // PPU Data
			// Reads from the NameTable RAM get delayed one cycle.
			data = ppu_data_buffer;
			ppu_data_buffer = ppuRead(vram_addr.reg);

			// However, if the address was in the palette range, the read is immediate
			if (vram_addr.reg >= 0x3F00) data = ppu_data_buffer;

			vram_addr.reg += (control.increment_mode ? 32 : 1);
			break;
		}
	}

	return data;
}

void PPU2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr)
	{
	case 0x0000: // Control
		control.reg = data;
		tram_addr.nametable_x = control.nametable_x;
		tram_addr.nametable_y = control.nametable_y;
		break;
	case 0x0001: // Mask
		mask.reg = data;
		break;
	case 0x0002: // Status
		// Not writable
		break;
	case 0x0003: // OAM Address
		oam_addr = data;
		break;
	case 0x0004: // OAM Data
		pOAM[oam_addr] = data;
		break;
	case 0x0005: // Scroll
		if (addr_latch == 0)
		{
			fine_x = data & 0x07;
			tram_addr.coarse_x = data >> 3;
			addr_latch = 1;
		}
		else
		{
			tram_addr.fine_y = data & 0x07;
			tram_addr.coarse_y = data >> 3;
			addr_latch = 0;
		}
		break;
	case 0x0006: // PPU Address
		if (addr_latch == 0)
		{
			tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
			addr_latch = 1;
		}
		else
		{
			tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
			vram_addr = tram_addr;
			addr_latch = 0;
		}
		break;
	case 0x0007: // PPU Data
		ppuWrite(vram_addr.reg, data);
		vram_addr.reg += (control.increment_mode ? 32 : 1);
		break;
	}
}

uint8_t PPU2C02::ppuRead(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (cart != nullptr && cart->ppuRead(addr, data))
	{
        //Cartridge has handled the read
	}
	else if(addr >= 0x0000 && addr <= 0x1FFF)
	{
		//Pattern Table
		data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if(addr >= 0x2000 && addr <= 0x3EFF)
	{
		//Nametables
		addr &= 0x0FFF;

		if(cart != nullptr && cart->getMirroringMode() == NTMIRROR::VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
		else if(cart != nullptr && cart->getMirroringMode() == NTMIRROR::HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
	}
	else if(addr >= 0x3F00 && addr <= 0x3FFF)
	{
		//Palettes
		addr &= 0x001F; // Mask first 5 bits (32 entries)
		if(addr == 0x0010) addr = 0x0000; // Mirror of 0x0010
		if(addr == 0x0014) addr = 0x0004; // Mirror of 0x0014
		if(addr == 0x0018) addr = 0x0008; // Mirror of 0x0018
		if(addr == 0x001C) addr = 0x000C; // Mirror of 0x001C
		data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
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
	else if(addr >= 0x0000 && addr <= 0x1FFF)
	{
		//Pattern Table
		tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if(addr >= 0x2000 && addr <= 0x3EFF)
	{
		//Nametables
		addr &= 0x0FFF;
		if(cart != nullptr && cart->getMirroringMode() == NTMIRROR::VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
		else if(cart != nullptr && cart->getMirroringMode() == NTMIRROR::HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
	}
	else if(addr >= 0x3F00 && addr <= 0x3FFF)
	{
		//Palettes;
		addr &= 0x001F; // Mask first 5 bits (32 entries)
		if(addr == 0x0010) addr = 0x0000; // Mirror of 0x0010
		if(addr == 0x0014) addr = 0x0004; // Mirror of 0x0014
		if(addr == 0x0018) addr = 0x0008; // Mirror of 0x0018
		if(addr == 0x001C) addr = 0x000C; // Mirror of 0x001C
		tblPalette[addr] = data;
	}
}

void PPU2C02::connectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cart = cartridge;
}

void PPU2C02::reset()
{
	// Reset internal registers
	fine_x = 0x00;
	addr_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;
}

void PPU2C02::clock()
{

	//Lambda helper functions//

	//Increment the background tile "pointer" one tile/column horizontally
	auto incrementScrollX = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			if (vram_addr.coarse_x == 31)
			{
				vram_addr.coarse_x = 0;
				vram_addr.nametable_x = ~vram_addr.nametable_x; //Flip nametable X
			}
			else
			{
				vram_addr.coarse_x++;
			}
		}
	};

	//Increment the background tile "pointer" one scanline vertically
	auto incrementScrollY = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			if (vram_addr.fine_y < 7)
			{
				vram_addr.fine_y++;
			}
			else
			{
				vram_addr.fine_y = 0;
				if (vram_addr.coarse_y == 29)
				{
					vram_addr.coarse_y = 0;
					vram_addr.nametable_y = ~vram_addr.nametable_y; //Flip nametable Y
				}
				else if (vram_addr.coarse_y == 31)
				{
					vram_addr.coarse_y = 0;
				}
				else
				{
					vram_addr.coarse_y++;
				}
			}
		}
	};

	//Transfer the temporarily stored horizontal nametable access information into the "pointer"
	auto transferAddressX = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			vram_addr.nametable_x = tram_addr.nametable_x;
			vram_addr.coarse_x = tram_addr.coarse_x;
		}
	};

	//Transfer the temporarily stored vertical nametable access information into the "pointer"
	auto transferAddressY = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			vram_addr.fine_y = tram_addr.fine_y;
			vram_addr.nametable_y = tram_addr.nametable_y;
			vram_addr.coarse_y = tram_addr.coarse_y;
		}
	};

	//Prime the "in-effect" background tile shifters ready for output
	auto loadBackgroundShifters = [&]()
	{
		bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
		bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

		bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
		bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
	};

	//Update shifters. Every cycle, shifters are clocked, and pixels are fetched from the shifter
	auto updateShifters = [&]()
	{
		if (mask.render_background)
		{
			bg_shifter_pattern_lo <<= 1;
			bg_shifter_pattern_hi <<= 1;

			bg_shifter_attrib_lo <<= 1;
			bg_shifter_attrib_hi <<= 1;
		}

		if (mask.render_sprites && cycle >= 1 && cycle < 258)
		{
			for (uint8_t i = 0; i < scanline_sprite_count; i++)
			{
				if (scanline_sprites[i].x > 0)
				{
					scanline_sprites[i].x--;
				}
				else
				{
					sprite_shifter_pattern_lo[i] <<= 1;
					sprite_shifter_pattern_hi[i] <<= 1;
				}
			}
		}
	};

	//End of Lambda helper functions//


	if(scanline >= -1 && scanline < 240)
	{
		if (scanline == 0 && cycle == 0)
		{
			// Skip cycle 0
			cycle = 1;
		}

		if (scanline == -1 && cycle == 1)
		{
			// Start of new frame
			status.vertical_blank = 0;
			status.sprite_zero_hit = 0;
			status.sprite_overflow = 0;

			for(int i = 0; i < 8; i++)
			{
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
		}

		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			updateShifters();

			switch ((cycle - 1) % 8)
			{
			case 0:
				loadBackgroundShifters();

				bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
				break;
			case 2:
				bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11) | (vram_addr.nametable_x << 10) | ((vram_addr.coarse_y >> 2) << 3) | (vram_addr.coarse_x >> 2));
				if (vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
				if (vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
				bg_next_tile_attrib &= 0x03;
				break;
			case 4:
				bg_next_tile_lsb = ppuRead((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + vram_addr.fine_y);
				break;
			case 6:
				bg_next_tile_msb = ppuRead((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + vram_addr.fine_y + 8);
				break;
			case 7:
				incrementScrollX();
				break;
			}
		}

		if (cycle == 256)
		{
			// End of scanline
			incrementScrollY();
		}

		if (cycle == 257)
		{
			loadBackgroundShifters();
			transferAddressX();
		}

		if (cycle == 338 || cycle == 340)
		{
			bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			// End of vertical blank period
			transferAddressY();
		}

		if (cycle == 257 && scanline >= 0)
		{
			// Sprite evaluation
			std::memset(scanline_sprites, 0xFF, 8*sizeof(PPUOAMEntry));
			scanline_sprite_count = 0;

			//Clear sprite pattern shifters
			for(int i = 0; i < 8; i++)
			{
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}

			uint8_t n = 0;
			sprite_zero_hit_possible = false;
			while(n < 64 && scanline_sprite_count < 9)
			{
				int16_t diff = (int16_t)scanline - (int16_t)OAM[n].y;
				if (diff >= 0 && diff < (control.sprite_size ? 16 : 8))
				{
					if (scanline_sprite_count < 8)
					{
						if (n == 0 ) sprite_zero_hit_possible = true;
						// Copy OAM entry
						scanline_sprites[scanline_sprite_count].y = OAM[n].y;
						scanline_sprites[scanline_sprite_count].id = OAM[n].id;
						scanline_sprites[scanline_sprite_count].attribute = OAM[n].attribute;
						scanline_sprites[scanline_sprite_count].x = OAM[n].x;
						scanline_sprite_count++;
					}
				}
				n++;
			}
			status.sprite_overflow = (scanline_sprite_count > 8);
		}

		if (cycle == 340)
		{
			for (uint8_t i=0; i<scanline_sprite_count; i++)
			{
				uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
				uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;

				if(!control.sprite_size)
				{
					// 8x8 sprite mode
					if (!(scanline_sprites[i].attribute & 0x80))
					{
						// Normal
						sprite_pattern_addr_lo = (
							(uint16_t)control.pattern_sprite << 12) |  // Sprite pattern table address
							((uint16_t)scanline_sprites[i].id << 4) |  // Sprite Tile ID * 16
							(scanline - scanline_sprites[i].y);        // Fine Y offset
					}
					else
					{
						// Vertically flipped
						sprite_pattern_addr_lo = (
							(uint16_t)control.pattern_sprite << 12) | 
							((uint16_t)scanline_sprites[i].id << 4) | 
							(7 - (scanline - scanline_sprites[i].y));  // Fine Y is reversed
					}
				}
				else
				{
					// 8x16 sprite mode
					if (!(scanline_sprites[i].attribute & 0x80))
					{
						// Normal
						if (scanline - scanline_sprites[i].y < 8)
						{
							// Top half of sprite
							sprite_pattern_addr_lo = (
								((scanline_sprites[i].id & 0x01) << 12) |     // Sprite pattern table address
								((scanline_sprites[i].id & 0xFE) << 4)  |     // Sprite Tile ID * 16
								((scanline - scanline_sprites[i].y) & 0x07)); // Fine Y offset
						}
						else
						{
							// Bottom half of sprite
							sprite_pattern_addr_lo = (
								((scanline_sprites[i].id & 0x01) << 12) |       // Sprite pattern table address
								(((scanline_sprites[i].id & 0xFE) + 1) << 4) |  // Sprite Tile ID * 16
								((scanline - scanline_sprites[i].y) & 0x07));   // Fine Y offset
						}
						
					}
					else
					{
						// Vertically flipped
						if (scanline - scanline_sprites[i].y < 8)
						{
							// Top half of sprite
							sprite_pattern_addr_lo = (
								((scanline_sprites[i].id & 0x01) << 12)      |
								(((scanline_sprites[i].id & 0xFE) + 1) << 4) |
								(7 - (scanline - scanline_sprites[i].y) & 0x07)); // Fine Y offset is reversed
						}
						else
						{
							// Bottom half of sprite
							sprite_pattern_addr_lo = (
								((scanline_sprites[i].id & 0x01) << 12) |     
								((scanline_sprites[i].id & 0xFE) << 4)  |     
								(7 - (scanline - scanline_sprites[i].y) & 0x07)); // Fine Y offset is reversed
						}
					}
				}

				sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;

				// Fetch the pattern from the pattern table
				sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
				sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);

				if (scanline_sprites[i].attribute & 0x40)
				{
					// Flip horizontally
					auto flipbyte = [](uint8_t b) -> uint8_t
					{
						b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
						b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
						b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
						return b;
					};

					sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
					sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
				}

				// Prime the sprite shifter with the pattern data
				sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
				sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
			}	
		}
	}

	if (scanline == 240) {
		// Do nothing
	}

	if (scanline >= 241 && scanline < 261)
	{
		if (scanline == 241 && cycle == 1)
		{
			// Set vertical blanking flag
			status.vertical_blank = 1;

			// If the control register tells us to generate an NMI
			if (control.enable_nmi)
			{
				// Signal NMI on next CPU clock
				signal_nmi = true;
			}
		}
	}

	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;

	if (mask.render_background)
	{
		uint16_t bit_mux = 0x8000 >> fine_x;
		uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
		uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

		bg_pixel = (p1_pixel << 1) | p0_pixel;

		// Get palette
		uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
		uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}

	uint8_t fg_pixel = 0x00;
	uint8_t fg_palette = 0x00;
	uint8_t fg_priority = 0x00;

	if (mask.render_sprites)
	{
		sprite_zero_being_rendered = false;

		for (uint8_t i = 0; i < scanline_sprite_count; i++)
		{
			if (scanline_sprites[i].x == 0)
			{
				
				uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

				fg_palette  = (scanline_sprites[i].attribute & 0x03) + 0x04;
				fg_priority = (scanline_sprites[i].attribute & 0x20) == 0;

				if (fg_pixel != 0)
				{
					if (i==0) sprite_zero_being_rendered = true;
					break;
				}
			}
		}
	}

	uint8_t pixel = 0x00;
	uint8_t palette = 0x00;

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		if (fg_priority)
		{
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			pixel = bg_pixel;
			palette = bg_palette;
		}

		if (sprite_zero_hit_possible && sprite_zero_being_rendered)
		{
			if (mask.render_background && mask.render_sprites)
			{
				if (~(mask.render_background_left | mask.render_sprites_left))
				{
					if (cycle >= 9 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
				else
				{
					if (cycle >= 1 && cycle < 258)
					{
						status.sprite_zero_hit = 1;
					}
				}
			}
		}
	}

	// Draw pixel
	Renderer::getInstance()->draw_pixel(cycle-1, scanline, getColorFromPalette(palette, pixel));

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

glm::vec3 PPU2C02::getColorFromPalette(uint8_t palette, uint8_t pixel)
{
	uint16_t a = ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F;
	auto c = palColors[a];
	return c;
}

std::vector<glm::vec3> PPU2C02::getPatternTable(uint8_t i, uint8_t palette)
{
	std::vector<glm::vec3> pattern_tbl_pixels;
	pattern_tbl_pixels.resize(128 * 128, glm::vec3(0.0f, 0.0f, 0.0f));

	for(uint16_t tileY = 0; tileY < 16; tileY++)
	{
		for(uint16_t tileX = 0; tileX < 16; tileX++)
		{
			uint16_t offset = tileY * 256 + tileX * 16;

			for(uint16_t row = 0; row < 8; row++)
			{
				uint8_t tile_lsb = ppuRead(i * 0x1000 + offset + row + 0x0000);
				uint8_t tile_msb = ppuRead(i * 0x1000 + offset + row + 0x0008);

				for(uint16_t col = 0; col < 8; col++)
				{
					uint8_t pixel = ((tile_lsb & 0x01) << 1) | (tile_msb & 0x01);
					tile_lsb >>= 1;
					tile_msb >>= 1;

					uint16_t ix = tileX * 8 + (7 - col);
					uint16_t iy = tileY * 8 + row;
					glm::vec3 color = getColorFromPalette(palette, pixel);

					pattern_tbl_pixels[ix + iy * 128] = color;
				}
			}
		}
	}
	return pattern_tbl_pixels;
}

void PPU2C02::printPalette()
{
	for(int i = 0; i < 32; i++)
	{
		spdlog::info("Palette[0x{:02X}] = 0x{:02X}", i, tblPalette[i]);
	}
}