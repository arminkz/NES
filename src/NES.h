#pragma once

#include "stdafx.h"

#include "utilities/Singleton.h"

#include "cpu6502.h"
#include "ppu2C02.h"
#include "APU2A03.h"
#include "Cartridge.h"

class NES : public Singleton<NES>{
    friend class Singleton<NES>;

public:
    NES();
    ~NES();

    bool emulationRun = false;

public:

    //Devices on the NES
    CPU6502 cpu;
    PPU2C02 ppu;
    
    //RAM on CPU bus
    std::array<uint8_t, 2048> cpuRam;

    //Controllers on CPU bus
    uint8_t controller[2];


//Audio
public:
    APU2A03 apu;
    double currAudioSample = 0.0;                  // Current Audio Sample
    static float soundOut(double dTime);           // Called by SoundEngine to get audio sample
    void setAudioSampleRate(uint32_t nSampleRate); // Must be called to syncronize with audio sample rate
private:
    double audioTime = 0.0;
    double audioTimePerSample = 0.0;
    double audioTimePerNESClock = 0.0;


public: //CPU Bus Read/Write
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);

public: //System Interface
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    bool isCartridgeInserted();
    void reset();
    bool clock();
    void update();
    void updateWithAudio();



public:
    // Developer Tools
    uint8_t dev_active_palette = 0x00;

    //Disassembled code for debugging
    std::map<uint16_t, std::string> mapAsm;

private:
    uint32_t nesClockCounter = 0;
    Time lastSystemTime = Clock::now();
    float residualTime = 0;

    // Internal cache of controller state
	uint8_t controller_state[2];

private:
    std::shared_ptr<Cartridge> cart;

    // Key states        X      Z      A      S      UP     DOWN   LEFT   RIGHT
    bool key_state[8] = {false, false, false, false, false, false, false, false};

    // Direct Memory Access (DMA)
    uint8_t dma_page = 0x00;
    uint8_t dma_addr = 0x00;
    uint8_t dma_data = 0x00;
    bool dma_transfer = false;
    bool dma_stall = true;
    
public:
    // [Callbacks (GLFW)]
	static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void mouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	//static void mouseCursorEvent(GLFWwindow* window, double xpos, double ypos);
	//static void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	//static void resizeEvent(GLFWwindow* window, int width, int height);
	//static void windowRefresh(GLFWwindow* window);

    // [Higher Level Callbacks]
	void processPressedKeyEvent(const int key, const int mods);
	void processReleasedKeyEvent(const int key, const int mods);
	//void processMouseDrag(const int xOffset, const int yOffset);

};