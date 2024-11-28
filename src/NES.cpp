#include "NES.h"

NES::NES() {
    //Clear RAM contents
    for (uint8_t &i : ram) i = 0x00;
}

NES::~NES() {

}

void NES::write(uint16_t addr, uint8_t data)
{  
    //Write to RAM
    if (addr >= 0x0000 && addr <= 0xFFFF)
        ram[addr] = data;
}

uint8_t NES::read(uint16_t addr, bool readOnly)
{
    //Read from RAM
    if (addr >= 0x0000 && addr <= 0xFFFF)
        return ram[addr];
    return 0x00;
}
