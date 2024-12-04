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
    //Write to RAM
    if (addr >= 0x0000 && addr <= 0x1FFF)
        cpuRam[addr & 0x07FF] = data; //mirroring
}

uint8_t NES::cpuRead(uint16_t addr, bool readOnly)
{
    //Read from RAM
    if (addr >= 0x0000 && addr <= 0x1FFF)
        return cpuRam[addr & 0x07FF]; //mirroring
    return 0x00;
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
