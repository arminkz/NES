#include "stdafx.h"

#include "Window.h"

//Use NVIDIA graphic card instead of Intel (Windows only)
#ifdef _WIN32
#include <windows.h> // DWORD is undefined otherwise
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

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

