#include "Mapper.h"

Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks)
{
	nPrgBanks = prgBanks;
	nChrBanks = chrBanks;

	reset();
}

Mapper::~Mapper()
{
}

void Mapper::reset()
{
}

NTMIRROR Mapper::getMirroringMode()
{
	return NTMIRROR::HARDWARE;
}