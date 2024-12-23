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

	void render_game(uint16_t width, uint16_t height); // Called every frame by ImGui to render the texture to the screen
	void render_pattern_tables(uint16_t width, uint16_t height);

	void refresh_game_screen(); // Called when changes are ready to be displayed
	void refresh_pattern_tables_screen();

private:

	int32_t screen_w;
	int32_t screen_h;
	int32_t pixel_w;
	int32_t pixel_h;

	//Game screen pixels
	std::vector<glm::vec3> pixels;

	//dummy screen for pattern tables
	std::vector<glm::vec3> pattern_tbl_pixels;

	GLFWwindow* _window;

	GLuint VAO; // vertex array object
	GLuint VBO; // vertex buffer object
	GLuint EBO; // element buffer object

	GLuint FBO; // frame buffer object
	GLuint RBO; // rendering buffer object
	GLuint pattern_tbl_FBO;
	GLuint pattern_tbl_RBO;

	GLuint game_tex_id;         // the texture we use for rendering
	GLuint game_out_tex_id;     // the texture id we'll render to and pass to ImGui

	GLuint pattern_tbl_tex_id;
	GLuint pattern_tbl_out_tex_id;

	GLuint shader_program; // the shader id

	void create_triangle();

	void update_triangle(int32_t viewport_w, int32_t viewport_h, int32_t screen_w, int32_t screen_h);

	void add_shader(GLuint program, const char* shader_code, GLenum type);

	void create_shaders();

	void create_game_framebuffer();

	void rescale_game_framebuffer(float width, float height);

	void create_pattern_tables_framebuffer();

	void rescale_pattern_tables_framebuffer(float width, float height);

	void load_input_texture(const char* filename);

public:
	// Drawing functions

	void draw_pixel(int32_t x, int32_t y, glm::vec3 color);

};