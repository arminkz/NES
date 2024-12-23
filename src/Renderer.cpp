#include "Renderer.h"

#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/// [Protected methods]

Renderer::Renderer()
{}

/// [Public methods]

Renderer::~Renderer()
{
	//TODO: shouldn't do anything if the class is not initialized.
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &game_out_tex_id);
	glDeleteRenderbuffers(1, &RBO);
}

void Renderer::initialize(GLFWwindow* window, int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h)
{
	this->_window = window;

	this->screen_w = screen_w;
	this->screen_h = screen_h;
	this->pixel_w = pixel_w;
	this->pixel_h = pixel_h;

	// Initialize Pixels with black color
	pixels.resize(screen_w * screen_h, glm::vec3(0.0f, 0.0f, 0.0f));

	pattern_tbl_pixels.resize(256 * 128, glm::vec3(0.0f, 1.0f, 0.0f));

	for (int32_t x = 0; x < screen_w; x++)
	{
		for (int32_t y = 0; y < screen_h; y++)
		{
			auto color = glm::vec3(
				(float)rand() / RAND_MAX,
				(float)rand() / RAND_MAX,
				(float)rand() / RAND_MAX
			);
			pixels[x + y * screen_w] = color;
		}
	}

	// Generate input texture that will be filled by PPU
	glGenTextures(1, &game_tex_id);
	glGenTextures(1, &pattern_tbl_tex_id);

	refresh_game_screen();
	refresh_pattern_tables_screen();

	create_triangle();
	create_shaders();
	create_game_framebuffer();
	create_pattern_tables_framebuffer();
}

//Used to draw renderables inside one ImGui window
//TODO: Refactor render_game and render_pattern_tables to use a more generic approach
void Renderer::render_game(uint16_t width, uint16_t height)
{
	rescale_game_framebuffer(width, height);
	glViewport(0, 0, width, height);
	update_triangle(width, height, screen_w, screen_h);

	ImVec2 pos = ImGui::GetCursorScreenPos();
		
	ImGui::GetWindowDrawList()->AddImage(
		(ImTextureID)(intptr_t)game_out_tex_id, 
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + width, pos.y + height), 
		ImVec2(0, 1), 
		ImVec2(1, 0)
	);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		
	glUseProgram(shader_program);                          // Bind shader program

	glActiveTexture(GL_TEXTURE0);                          // Bind texture
    glBindTexture(GL_TEXTURE_2D, game_tex_id);

	glBindVertexArray(VAO);                                // Bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);                    // Bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);            // Bind EBO

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);  // Draw Elements

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);              // Unbind EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);                      // Unbind VBO
	glBindVertexArray(0);                                  // Unbind VAO

	glBindTexture(GL_TEXTURE_2D, 0);                       // Unbind texture
	glDisable( GL_TEXTURE_2D );

	glUseProgram(0);                                       // Unbind shader program
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::render_pattern_tables(uint16_t width, uint16_t height)
{
	rescale_pattern_tables_framebuffer(width, height);
	glViewport(0, 0, width, height);
	update_triangle(width, height, 256, 128);

	ImVec2 pos = ImGui::GetCursorScreenPos();
		
	ImGui::GetWindowDrawList()->AddImage(
		(ImTextureID)(intptr_t)pattern_tbl_out_tex_id, 
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + width, pos.y + height), 
		ImVec2(0, 1), 
		ImVec2(1, 0)
	);

	glBindFramebuffer(GL_FRAMEBUFFER, pattern_tbl_FBO);
		
	glUseProgram(shader_program);                          // Bind shader program

	glActiveTexture(GL_TEXTURE0);                          // Bind texture
	glBindTexture(GL_TEXTURE_2D, pattern_tbl_tex_id);

	glBindVertexArray(VAO);                                // Bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);                    // Bind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);            // Bind EBO

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);  // Draw Elements

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);              // Unbind EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);                      // Unbind VBO
	glBindVertexArray(0);                                  // Unbind VAO

	glBindTexture(GL_TEXTURE_2D, 0);                       // Unbind texture
	glDisable( GL_TEXTURE_2D );

	glUseProgram(0);                                       // Unbind shader program
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::update_triangle(int32_t viewport_w, int32_t viewport_h, int32_t screen_w, int32_t screen_h)
{
	float screenAspect = float(screen_w) / float(screen_h);
	float viewportAspect = float(viewport_w) / float(viewport_h);

	float quadWidth = 1.0f;
	float quadHeight = 1.0f;

	// Adjust the quad's scale:
	if (screenAspect > viewportAspect) {
		quadHeight = viewportAspect / screenAspect;
	} else {
		quadWidth = screenAspect / viewportAspect;
	}

	float vertices[] = {
		// Positions         // Texture Coords
		-quadWidth, -quadHeight, 0.0f,  0.0f, 1.0f,  // Bottom-left
		 quadWidth, -quadHeight, 0.0f,  1.0f, 1.0f,  // Bottom-right
		 quadWidth,  quadHeight, 0.0f,  1.0f, 0.0f,  // Top-right
		-quadWidth,  quadHeight, 0.0f,  0.0f, 0.0f   // Top-left
	};

	glBindVertexArray(VAO); 	        // Bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);   // Unbind VBO
	glBindVertexArray(0); 	            // Unbind VAO
}

void Renderer::create_triangle()
{
	float vertices[] = {
    // Positions         // Texture Coords
    -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // Bottom-left
     1.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // Bottom-right
     1.0f,  1.0f, 0.0f,  1.0f, 0.0f,  // Top-right
    -1.0f,  1.0f, 0.0f,  0.0f, 0.0f   // Top-left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &VAO); // Generate VAO
	glBindVertexArray(VAO); 	// Bind VAO

	glGenBuffers(1, &VBO);              // Generate VBO
	glGenBuffers(1, &EBO);              // Generate EBO

	glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        // Bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                      // Bind EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copy indices to EBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);      // Define a vertex attribute pointer for XYZ // 0 is the index of the first vertex attribute
	glEnableVertexAttribArray(0);                                                    // Enable the vertex attribute

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Define a vertex attribute pointer for UV (Texture coordinates)
	glEnableVertexAttribArray(1);                                                                   // Enable the vertex attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0);         // Unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO
	glBindVertexArray(0);                     // Unbind VAO
}

void Renderer::add_shader(GLuint program, const char* shader_code, GLenum type) 
{
	GLuint current_shader = glCreateShader(type);

	const GLchar* code[1];
	code[0] = shader_code;

	GLint code_length[1];
	code_length[0] = strlen(shader_code);

	glShaderSource(current_shader, 1, code, code_length);
	glCompileShader(current_shader);

	GLint result = 0;
	GLchar log[1024] = {0};

	glGetShaderiv(current_shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(current_shader, sizeof(log), NULL, log);
		spdlog::error("Error compiling {} shader: {}", type, log);
		return;
	}

	glAttachShader(program, current_shader);
}

void Renderer::create_shaders()
{
	shader_program = glCreateProgram();
	if(!shader_program) {
		spdlog::error("Error creating shader program!");
		exit(1);
	}

	const char* vertex_shader_code = R"*(
	#version 330
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTexCoord;

	out vec2 TexCoord;
	void main()
	{
		gl_Position = vec4(aPos, 1.0);  // Position
		TexCoord = aTexCoord;           // Pass texture coordinates to fragment shader
	}
	)*";

	const char* fragment_shader_code = R"*(
	#version 330

	in vec2 TexCoord;    // Texture coordinate input from vertex shader
	out vec4 FragColor;  // Final color output

	uniform sampler2D texture1;  // Texture uniform

	void main()
	{
		FragColor = texture(texture1, TexCoord);  // Sample the texture at the given coordinate
	}
	)*";

	add_shader(shader_program, vertex_shader_code, GL_VERTEX_SHADER);
	add_shader(shader_program, fragment_shader_code, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar log[1024] = {0};

	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader_program, sizeof(log), NULL, log);
		spdlog::error("Error linking program: {}", log);
		return;
	}

	glValidateProgram(shader_program);
	glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader_program, sizeof(log), NULL, log);
		spdlog::error("Error validating program: {}", log);
		return;
	}

	glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0); // Set texture uniform
}

void Renderer::create_game_framebuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	uint16_t initial_width = 800;
	uint16_t initial_height = 600;

	glGenTextures(1, &game_out_tex_id);
	glBindTexture(GL_TEXTURE_2D, game_out_tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, initial_width, initial_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, game_out_tex_id, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, initial_width, initial_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("[FRAMEBUFFER] Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Unbind FBO
	glBindTexture(GL_TEXTURE_2D, 0);        // Unbind texture
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind RBO
}

void Renderer::rescale_game_framebuffer(float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, game_out_tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, game_out_tex_id, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

void Renderer::create_pattern_tables_framebuffer()
{
	glGenFramebuffers(1, &pattern_tbl_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, pattern_tbl_FBO);

	uint16_t initial_width = 800;
	uint16_t initial_height = 600;

	glGenTextures(1, &pattern_tbl_out_tex_id);
	glBindTexture(GL_TEXTURE_2D, pattern_tbl_out_tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, initial_width, initial_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pattern_tbl_out_tex_id, 0);

	glGenRenderbuffers(1, &pattern_tbl_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, pattern_tbl_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, initial_width, initial_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pattern_tbl_RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("[FRAMEBUFFER] Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Unbind FBO
	glBindTexture(GL_TEXTURE_2D, 0);        // Unbind texture
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind RBO
}

void Renderer::rescale_pattern_tables_framebuffer(float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, pattern_tbl_out_tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pattern_tbl_out_tex_id, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, pattern_tbl_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pattern_tbl_RBO);
}

void Renderer::load_input_texture(const char* filename)
{
	// Set texture parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image using stb_image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		spdlog::info("No. of channels: {}", nrChannels);
		spdlog::info("Texture loaded successfully");
    }
    else
    {
		spdlog::error("Failed to load texture");
    }
    stbi_image_free(data);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::refresh_game_screen()
{
	glBindTexture(GL_TEXTURE_2D, game_tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_w, screen_h, 0, GL_RGB, GL_FLOAT, pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::refresh_pattern_tables_screen()
{
	glBindTexture(GL_TEXTURE_2D, pattern_tbl_tex_id);
	//Should read pattern tables from PPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 128, 0, GL_RGB, GL_FLOAT, pattern_tbl_pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//////////////////////////////////////////////////////////////////////////
// Drawing functions

void Renderer::draw_pixel(int32_t x, int32_t y, glm::vec3 color)
{
	// Check if the pixel is out of bounds
	if (x >= 0 && x < screen_w && y >= 0 && y < screen_h)
	{
		//row major indexing
		pixels[y*screen_w + x] = color;
	}
}