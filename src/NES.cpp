#include "NES.h"

#include "Renderer.h"
#include "InputManager.h"

NES::NES() 
{
    //Clear RAM contents
    for (uint8_t &i : cpuRam) i = 0x00;

    //Connect CPU to NES Bus
    cpu.connectBus(this);
}

NES::~NES() 
{

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
    else if((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017)
    {
        //Write to APU
        apu.cpuWrite(addr, data);
    }
    else if(addr == 0x4014)
    {
        //Activate DMA transfer
        dma_page = data;
        dma_addr = 0x00;
        dma_transfer = true;
    }
    else if(addr >= 0x4016 && addr <= 0x4017)
    {
        //Write to controller means update controller state
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
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
    else if(addr >= 0x4016 && addr <= 0x4017)
    {
        //Read from controller
        data = (controller_state[addr & 0x0001] & 0x80) > 0;
		controller_state[addr & 0x0001] <<= 1;
    }
    return data;
}

void NES::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}

bool NES::isCartridgeInserted()
{
    return cart != nullptr;
}

void NES::updateWithAudio()
{
    Renderer::getInstance()->refresh_game_screen();
    Renderer::getInstance()->refresh_pattern_tables_screen();
}

void NES::update()
{
    fsec elapsed = Clock::now() - lastSystemTime;
    lastSystemTime = Clock::now();

    //Called every GLFW frame
    if (emulationRun)
    {
        if (residualTime > 0)
        {
            residualTime -= elapsed.count();
        }
        else
        {
            residualTime += (1.0f / 60.0f) - elapsed.count();
            do { clock(); } while (!ppu.frameComplete);
            ppu.frameComplete = false;
            Renderer::getInstance()->refresh_game_screen();
            Renderer::getInstance()->refresh_pattern_tables_screen(); //TODO: Not necessary if the pattern tables are not being displayed
        }
    }
}

void NES::reset()
{
    if (cart != nullptr) cart->reset();
    cpu.reset();
    ppu.reset();
    nesClockCounter = 0;
    
    dma_page = 0x00;
    dma_addr = 0x00;
    dma_data = 0x00;
    dma_stall = true;
    dma_transfer = false;
}

bool NES::clock()
{
    // Dont do anything if Emulation is paused
    if (emulationRun == false) return true;

    // Update controller state (only 1 controller for now)
    controller[0] = 0x00;
    controller[0] |= key_state[0] ? 0x80 : 0x00;
    controller[0] |= key_state[1] ? 0x40 : 0x00;
    controller[0] |= key_state[2] ? 0x20 : 0x00;
    controller[0] |= key_state[3] ? 0x10 : 0x00;
    controller[0] |= key_state[4] ? 0x08 : 0x00;
    controller[0] |= key_state[5] ? 0x04 : 0x00;
    controller[0] |= key_state[6] ? 0x02 : 0x00;
    controller[0] |= key_state[7] ? 0x01 : 0x00;

    controller[0] |= InputManager::getInstance()->joystick1Buttons[0] ? 0x80 : 0x00;
    controller[0] |= InputManager::getInstance()->joystick1Buttons[1] ? 0x40 : 0x00;
    controller[0] |= InputManager::getInstance()->joystick1Buttons[2] ? 0x20 : 0x00;
    controller[0] |= InputManager::getInstance()->joystick1Buttons[3] ? 0x10 : 0x00;
    controller[0] |= InputManager::getInstance()->joystick1Directions[0] ? 0x08 : 0x00; // UP
    controller[0] |= InputManager::getInstance()->joystick1Directions[1] ? 0x04 : 0x00; // DOWN
    controller[0] |= InputManager::getInstance()->joystick1Directions[2] ? 0x02 : 0x00; // LEFT
    controller[0] |= InputManager::getInstance()->joystick1Directions[3] ? 0x01 : 0x00; // RIGHT

    controller[1] = 0x00;

    controller[1] |= InputManager::getInstance()->joystick2Buttons[0] ? 0x80 : 0x00;
    controller[1] |= InputManager::getInstance()->joystick2Buttons[1] ? 0x40 : 0x00;
    controller[1] |= InputManager::getInstance()->joystick2Buttons[2] ? 0x20 : 0x00;
    controller[1] |= InputManager::getInstance()->joystick2Buttons[3] ? 0x10 : 0x00;
    controller[1] |= InputManager::getInstance()->joystick2Directions[0] ? 0x08 : 0x00; // UP
    controller[1] |= InputManager::getInstance()->joystick2Directions[1] ? 0x04 : 0x00; // DOWN
    controller[1] |= InputManager::getInstance()->joystick2Directions[2] ? 0x02 : 0x00; // LEFT
    controller[1] |= InputManager::getInstance()->joystick2Directions[3] ? 0x01 : 0x00; // RIGHT

    // PPU Clock
    ppu.clock();

    // APU Clock
    apu.clock();

    // CPU clock (1/3 * System Clock)
    if (nesClockCounter % 3 == 0)
    {
        if(dma_transfer)
        {
            if(dma_stall) {
                if (nesClockCounter % 2 == 1) dma_stall = false; // Wait for syncronization with CPU Clock
            } else {
                if (nesClockCounter % 2 == 0) {
                    //On even cycles, read from CPU Bus
                    dma_data = cpuRead((uint16_t)(dma_page << 8) | dma_addr);
                } else {
                    //On odd cycles, write to PPU OAM
                    ppu.pOAM[dma_addr] = dma_data;
                    dma_addr++;
                    //DMA transfer complete
                    if(dma_addr == 0x00) {
                        dma_transfer = false;
                        dma_stall = true;
                    }
                }
            }
        }
        else
        {
            cpu.clock();
        }
    }

    //Syncronize with Audio
    //Determines if an audio sample is ready to be output
    bool audioSampleReady = false;
    audioTime += audioTimePerNESClock;
    if (audioTime >= audioTimePerSample)
    {
        audioTime -= audioTimePerSample;
        currAudioSample = apu.getOutputSample();
        audioSampleReady = true;
    }

    // Check if PPU has signaled an NMI
    if (ppu.signal_nmi)
    {
        ppu.signal_nmi = false;
        cpu.nmi();
    }

    // Advance counter;;;
    nesClockCounter++;

    return audioSampleReady;
}

float NES::soundOut(double dTime)
{
    // Syncronizing to the Audio
    // Do enough clocking to match the audio sample rate
    // fsec elapsed = Clock::now() - NES::getInstance()->lastAudioTime;
    // if (elapsed.count() > NES::getInstance()->audioTimePerSample)
    // {
    //     spdlog::info("Time : {}  Expected: {}", elapsed.count(), NES::getInstance()->audioTimePerSample);
    // }
    // NES::getInstance()->lastAudioTime = Clock::now();

    while(!NES::getInstance()->clock()) {}; //Clock until an audio sample is ready
    //do { NES::getInstance()->clock(); } while (!NES::getInstance()->ppu.frameComplete); //Clock until the frame is complete
    return static_cast<float>(NES::getInstance()->currAudioSample);
}   

void NES::setAudioSampleRate(uint32_t nSampleRate)
{
    audioTimePerSample = 1.0 / (double)nSampleRate;
    audioTimePerNESClock = 1.0 / 5369318.0; //PPU Clock Rate
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
            reset();
            break;

        case GLFW_KEY_C:
            //Clock NES
            clock();
            //Refresh screen
            Renderer::getInstance()->refresh_game_screen();
            Renderer::getInstance()->refresh_pattern_tables_screen();
            break;

        case GLFW_KEY_I:
            //Clock enough times to execute an CPU instruction
            do { clock(); } while (!cpu.complete());
            do { clock(); } while (cpu.complete());

            //Refresh screen
            Renderer::getInstance()->refresh_game_screen();
            Renderer::getInstance()->refresh_pattern_tables_screen();
            break;

        case GLFW_KEY_F:
            //Clock enough times to draw a frame
            do { clock(); } while (!ppu.frameComplete);
            //Use residual clocks to complete current cpu instruction
            do { clock(); } while (!cpu.complete());

            //Reset frame complete flag
            ppu.frameComplete = false;

            //Refresh screen
            Renderer::getInstance()->refresh_game_screen();
            Renderer::getInstance()->refresh_pattern_tables_screen();
            break;


        case GLFW_KEY_P:
            //Change active palette
            (++dev_active_palette) &= 0x07;
            //Refresh screen
            Renderer::getInstance()->refresh_pattern_tables_screen();
            break;

        case GLFW_KEY_O:
            NES::getInstance()->ppu.printPalette();
            break;

        case GLFW_KEY_SPACE:
            //Toggle emulation run
            emulationRun = !emulationRun;
            break;

        //////////////////////////////////////////
        // NES Controller Key Mappings
        case GLFW_KEY_X:
            key_state[0] = true;
            break;

        case GLFW_KEY_Z:
            key_state[1] = true;
            break;

        case GLFW_KEY_A:
            key_state[2] = true;
            break;

        case GLFW_KEY_S:
            key_state[3] = true;
            break;

        case GLFW_KEY_UP:
            key_state[4] = true;
            break;

        case GLFW_KEY_DOWN:
            key_state[5] = true;
            break;
            
        case GLFW_KEY_LEFT:
            key_state[6] = true;
            break;

        case GLFW_KEY_RIGHT:
            key_state[7] = true;
            break;   
    }
}

void NES::processReleasedKeyEvent(const int key, const int mods)
{
    switch (key)
    {
    //////////////////////////////////////////
    // NES Controller Key Mappings

    case GLFW_KEY_X:
        key_state[0] = false;
        break;

    case GLFW_KEY_Z:
        key_state[1] = false;
        break;

    case GLFW_KEY_A:
        key_state[2] = false;
        break;

    case GLFW_KEY_S:
        key_state[3] = false;
        break;

    case GLFW_KEY_UP:
        key_state[4] = false;
        break;

    case GLFW_KEY_DOWN:
        key_state[5] = false;
        break;

    case GLFW_KEY_LEFT:
        key_state[6] = false;
        break;

    case GLFW_KEY_RIGHT:
        key_state[7] = false;
        break;
    }
}
