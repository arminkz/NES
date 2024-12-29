#include "Cartridge.h"

#include "mappers/Mapper_000.h"

Cartridge::Cartridge(const std::string& fileName)
{
    // iNES Format Header
    // https://www.nesdev.org/wiki/INES
	struct INESHeader
	{
		char name[4];           //NES^1A
		uint8_t prg_rom_chunks; //in 16KB units
		uint8_t chr_rom_chunks; //in 8KB units
		uint8_t mapper1;        //mirroring, battery, trainer, four screen
		uint8_t mapper2;        //VS/Playchoice, NES 2.0
		uint8_t prg_ram_size;   //in 8KB units
		uint8_t tv_system1;     //0=NTSC, 1=PAL
		uint8_t tv_system2;     //0=NTSC, 1=PAL
		char unused[5];         //unused padding
	} header;

	std::ifstream ifs;
	ifs.open(fileName, std::ifstream::binary);
	if (ifs.is_open())
	{
		// Read file header
		ifs.read((char*)&header, sizeof(INESHeader));

		// If a "trainer" exists we just need to read past
		// it before we get to the good stuff
		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::cur);

		// Determine Mapper ID & Mirroring Type
		mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		hw_mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// "Discover" File Format
		uint8_t nFileType = 1;
		if ((header.mapper2 & 0x0C) == 0x08) nFileType = 2;

		if (nFileType == 0)
		{
			spdlog::info("File format: iNES type 0");
			spdlog::error("INES type 0 format not supported yet.");
		}

		if (nFileType == 1)
		{
			spdlog::info("File format: iNES type 1");
			nPrgBanks = header.prg_rom_chunks;
			prgMemory.resize(nPrgBanks * 16384); // 16 KB per bank
			ifs.read((char*)prgMemory.data(), prgMemory.size());

			nChrBanks = header.chr_rom_chunks;
			if (nChrBanks == 0)
			{
				// Create CHR RAM
				chrMemory.resize(8192);
			}
			else
			{
				// Create CHR ROM
				chrMemory.resize(nChrBanks * 8192);  // 8 KB per bank
			}
			ifs.read((char*)chrMemory.data(), chrMemory.size());
		}

		if (nFileType == 2)
		{
			spdlog::info("File format: iNES type 2");
			nPrgBanks = ((header.prg_ram_size & 0x07) << 8) | header.prg_rom_chunks;
			prgMemory.resize(nPrgBanks * 16384);
			ifs.read((char*)prgMemory.data(), prgMemory.size());

			nChrBanks = ((header.prg_ram_size & 0x38) << 8) | header.chr_rom_chunks;
			chrMemory.resize(nChrBanks * 8192);
			ifs.read((char*)chrMemory.data(), chrMemory.size());
		}
        
		// Print prg and chr rom chunks
		spdlog::info("PRG ROM chunks: {}", (int)header.prg_rom_chunks);
		spdlog::info("CHR ROM chunks: {}", (int)header.chr_rom_chunks);
		spdlog::info("Mapper ID: {}", mapperID);
		spdlog::info("Hardware NT Mirroring: {}", hw_mirror == HORIZONTAL ? "Horizontal" : "Vertical");

		// Load appropriate mapper
		switch (mapperID)
		{
		case 0: pMapper = std::make_shared<Mapper_000>(nPrgBanks, nChrBanks); break;
		}

		ifs.close();
	}
}

Cartridge::~Cartridge()
{
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapRead(addr, mapped_addr, data))
	{
		if (mapped_addr == 0xFFFFFFFF) // Mapper has set the data value
			return true;
		
		data = prgMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapWrite(addr, mapped_addr, data))
	{
		if (mapped_addr == 0xFFFFFFFF) // Mapper has set the data value
			return true;
		
		prgMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t & data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		data = chrMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		chrMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

void Cartridge::reset()
{
	if (pMapper != nullptr)
		pMapper->reset();
}

NTMIRROR Cartridge::getMirroringMode()
{
	NTMIRROR mapper_mirror = pMapper->getMirroringMode();
	return mapper_mirror == HARDWARE ? hw_mirror : mapper_mirror;
}