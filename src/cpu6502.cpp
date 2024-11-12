#include "cpu6502.h"

CPU6502::CPU6502()
{

}

CPU6502::~CPU6502()
{
    
}

uint8_t CPU6502::read(uint16_t addr)
{
    return bus->read(addr, false);
}

void CPU6502::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}
