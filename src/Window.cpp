#include "Window.h"

#include "stdafx.h"


#include "GUI.h"
#include "NES.h"
#include "Renderer.h"


Window::Window() : Singleton(), _size(glm::ivec2(1,1)), _window(nullptr), _windowState(NOT_LOADED)
{
}

void Window::changedSize(const uint16_t width, const uint16_t height)
{
	_size = glm::ivec2(width, height);
}

bool Window::initialize(const std::string& title, const uint16_t width, const uint16_t height, const uint8_t openGL4Version)
{
	_size = glm::ivec2(width, height);

	if (glfwInit() != GLFW_TRUE)
	{
		return false;
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);										// Antialiasing
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);			// OpenGL Core Profile 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glm::clamp((int)openGL4Version, 1, 6));

	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (_window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(0); // Disable VSync Because we are handling it manually

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		spdlog::error("Error initializing GLEW.");
		return false;
	}

	spdlog::info("----------------------------------------");
	spdlog::info("GPU : {} ", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	spdlog::info("Vendor : {} ", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	spdlog::info("Version : {} ", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	spdlog::info("Shading Language Version : {} ", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	spdlog::info("----------------------------------------");

	// Callbacks to main events
	//glfwSetWindowRefreshCallback(_window, InputManager::windowRefresh);
	//glfwSetFramebufferSizeCallback(_window, InputManager::resizeEvent);
	glfwSetKeyCallback(_window, NES::keyEvent);
	//glfwSetMouseButtonCallback(_window, InputManager::mouseButtonEvent);
	//glfwSetCursorPosCallback(_window, InputManager::mouseCursorEvent);
	//glfwSetScrollCallback(_window, InputManager::mouseScrollEvent);
	//spdlog::info("Input callbacks are set.");

	// OpenGL environment
	Renderer::getInstance()->initialize(_window, 256, 240, 1, 1);
	GUI::getInstance()->initialize(_window, openGL4Version);

	//InputManager::getInstance()->initialize(_window);

	_windowState = SUCCESSFUL_LOAD;

	return true;
}

void Window::startRenderingCycle()
{
	if (_windowState != SUCCESSFUL_LOAD) return;

	//InputManager* inputManager = InputManager::getInstance();

	while (!glfwWindowShouldClose(_window)) {

		NES::getInstance()->update();
		GUI::getInstance()->render();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	spdlog::info("Terminating GLFW window...");
	glfwDestroyWindow(_window);
	glfwTerminate();
}