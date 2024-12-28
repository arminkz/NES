#include "stdafx.h"

#include "Window.h"
#include <windows.h> // DWORD is undefined otherwise

//Use NVIDIA graphic card instead of Intel
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(int argc, char* argv[])
{
	srand(time(nullptr));
	spdlog::info("NES Emulator v0.1 - by Armin Kazemi @arminkz");

	const std::string title = "NES Emulator v0.1"; 
	const uint16_t width = 1550, height = 850;
	const auto window = Window::getInstance();
	
	if (const bool success = window->initialize(title, width, height))
	{
		window->startRenderingCycle();
	}
	else
	{
		spdlog::error("Failed to initialize GLFW.");
	}
	
	return 0;
}

