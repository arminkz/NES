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

		// Determine Mapper ID
		mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

		// "Discover" File Format
		uint8_t nFileType = 1;

		if (nFileType == 0)
		{
			spdlog::error("INES type 0 format not supported yet.");
		}

		if (nFileType == 1)
		{
			nPrgBanks = header.prg_rom_chunks;
			prgMemory.resize(nPrgBanks * 16384); // 16 KB per bank
			ifs.read((char*)prgMemory.data(), prgMemory.size());

			nChrBanks = header.chr_rom_chunks;
			chrMemory.resize(nChrBanks * 8192);  // 8 KB per bank
			ifs.read((char*)chrMemory.data(), chrMemory.size());
		}

		if (nFileType == 2)
		{
			spdlog::error("INES type 2 format not supported yet.");
		}

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
	if (pMapper->cpuMapRead(addr, mapped_addr))
	{
		data = prgMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapWrite(addr, mapped_addr))
	{
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
