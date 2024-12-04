#pragma once

#include "stdafx.h"

#include "utilities/Singleton.h"

#include "cpu6502.h"

class NES : public Singleton<NES>{
    friend class Singleton<NES>;

public:
    NES();
    ~NES();

    //Devices on the NES
    CPU6502 cpu;
    
    //RAM on CPU bus
    std::array<uint8_t, 2048> cpuRam;

public: //Bus Read/Write
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);


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