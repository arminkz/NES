#pragma once
#include "stdafx.h"

#include "utilities/Singleton.h"

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;

protected:
	Renderer();

public:

	virtual ~Renderer();

	void initialize(GLFWwindow* window);

	void render(uint16_t width, uint16_t height);

	// void sizeChanged(const uint16_t width, const uint16_t height);

private:

	GLFWwindow* _window;

	GLuint VAO; // vertex array object
	GLuint VBO; // vertex buffer object
	GLuint FBO; // frame buffer object
	GLuint RBO; // rendering buffer object
	GLuint texture_id; // the texture id we'll need later to create a texture
	GLuint shader_program; // the shader id

	void create_triangle();

	void add_shader(GLuint program, const char* shader_code, GLenum type);

	void create_shaders();

	void create_framebuffer();

	void bind_framebuffer();

	void unbind_framebuffer();

	void rescale_framebuffer(float width, float height);

};