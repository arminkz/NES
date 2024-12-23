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

	void initialize(GLFWwindow* window, int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h);

	int32_t get_screen_width() { return screen_w; }
	int32_t get_screen_height() { return screen_h; }

	void render(uint16_t width, uint16_t height); // Called every frame by ImGui to render the texture to the screen

	void refresh_screen(); // Called when changes are ready to be displayed

private:

	int32_t screen_w;
	int32_t screen_h;
	int32_t pixel_w;
	int32_t pixel_h;

	//std::vector<std::vector<glm::vec3>> pixels;

	//use 2d array to store pixel colors
	std::vector<glm::vec3> pixels;

	GLFWwindow* _window;

	GLuint VAO; // vertex array object
	GLuint VBO; // vertex buffer object
	GLuint EBO; // element buffer object
	GLuint FBO; // frame buffer object
	GLuint RBO; // rendering buffer object
	GLuint in_texture_id;  // the texture we use for rendering
	GLuint out_texture_id; // the texture id we'll render to and pass to ImGui
	GLuint shader_program; // the shader id

	void create_triangle();

	void update_triangle(int32_t viewport_w, int32_t viewport_h);

	void add_shader(GLuint program, const char* shader_code, GLenum type);

	void create_shaders();

	void create_framebuffer();

	void bind_framebuffer();

	void unbind_framebuffer();

	void rescale_framebuffer(float width, float height);

	void load_input_texture(const char* filename);

public:
	// Drawing functions

	void draw_pixel(int32_t x, int32_t y, glm::vec3 color);

};