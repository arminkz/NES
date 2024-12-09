#include "NES.h"

NES::NES() {
    //Clear RAM contents
    for (uint8_t &i : cpuRam) i = 0x00;

    //Connect CPU to NES Bus
    cpu.connectBus(this);
}

NES::~NES() {

}

void NES::cpuWrite(uint16_t addr, uint8_t data)
{  
    if(cart->cpuWrite(addr, data))
    {
        //Cartridge has handled the write
    }
    else if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        //Write to RAM on CPU Bus
        cpuRam[addr & 0x07FF] = data; //mirroring
    }
    else if(addr >= 0x2000 && addr <= 0x3FFF)
    {
        //Write to PPU Registers
        //Only 8 registers are addressable
        ppu.cpuWrite(addr & 0x0007, data);
    }
}

uint8_t NES::cpuRead(uint16_t addr, bool readOnly)
{
    uint8_t data = 0x00;

    if(cart->cpuRead(addr, data))
    {
        //Cartridge has handled the read
    }
    else if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        //Read from RAM on CPU Bus
        data = cpuRam[addr & 0x07FF]; //mirroring
    }
    else if(addr >= 0x2000 && addr <= 0x3FFF)
    {
        //Read from PPU Registers
        //Mirror every 8 bytes
        data = ppu.cpuRead(addr & 0x0007, readOnly);
    }

    return data;
}

void NES::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cart = cartridge;
    //ppu.connectCartridge(cartridge);
}

void NES::reset()
{
    cpu.reset();
    nSystemClockCounter = 0;
}

void NES::clock()
{
    // PPU has 3x clock frequency of CPU
    ppu.clock();

    if (nSystemClockCounter % 3 == 0)
    {
        cpu.clock();
    }

    nSystemClockCounter++;
}

//////////////////////////////////////////////////////////////////////////
// [Callbacks (GLFW)]

void NES::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    NES* nes = NES::getInstance();
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		nes->processPressedKeyEvent(key, mods);
	}
	else
	{
		nes->processReleasedKeyEvent(key, mods);
	}
}


//////////////////////////////////////////////////////////////////////////
// [Higher Level Callbacks]

void NES::processPressedKeyEvent(const int key, const int mods)
{
    switch(key) {
        case GLFW_KEY_R:
            //Reset
            cpu.reset();
            spdlog::info("Reset called.");
            break;
        case GLFW_KEY_C:
            //Clock CPU
            cpu.clock();
            spdlog::info("Clock called.");
            break;
        case GLFW_KEY_SPACE:
            //Clock until complete
            do
			{
				cpu.clock();
			} 
			while (!cpu.complete());
    }
    //Do something
}

void NES::processReleasedKeyEvent(const int key, const int mods)
{
    //Do something
}
