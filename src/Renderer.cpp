#include "Renderer.h"

#include "Window.h"

/// [Protected methods]

Renderer::Renderer()
{}

/// [Public methods]

Renderer::~Renderer()
{}

void Renderer::initialize()
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render()
{
	//Clean screen
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::sizeChanged(const uint16_t width, const uint16_t height)
{
	// tell opengl
	glViewport(0, 0, width, height);
}