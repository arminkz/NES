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
	glDeleteTextures(1, &out_texture_id);
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
	pixels.resize(screen_w, std::vector<glm::vec3>(screen_h, glm::vec3(0.0f, 0.0f, 0.0f)));

	// Set pixel values to random colors
	// Pixels are not 1x1 but pixel_w x pixel_h
	for (int32_t x = 0; x <= screen_w-pixel_w; x+=pixel_w)
	{
		for (int32_t y = 0; y <= screen_h-pixel_h; y+=pixel_h)
		{
			auto color = glm::vec3(
				(float)rand() / RAND_MAX,
				(float)rand() / RAND_MAX,
				(float)rand() / RAND_MAX
			);
			for (int32_t i = 0; i < pixel_w; i++)
			{
				for (int32_t j = 0; j < pixel_h; j++)
				{
					pixels[x + i][y + j] = color;
				}
			}
		}
	}

	// Load input texture
	load_input_texture("C:/Users/Armin/Pictures/nes.png");

	create_triangle();
	create_shaders();
	create_framebuffer();
}

//Used to draw renderables inside one ImGui window
void Renderer::render(uint16_t width, uint16_t height)
{
	rescale_framebuffer(width, height);
	glViewport(0, 0, width, height);
	update_triangle(width, height);

	refresh_screen();

	ImVec2 pos = ImGui::GetCursorScreenPos();
		
	ImGui::GetWindowDrawList()->AddImage(
		(ImTextureID)(intptr_t)out_texture_id, 
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + width, pos.y + height), 
		ImVec2(0, 1), 
		ImVec2(1, 0)
	);

	bind_framebuffer();
		
	glUseProgram(shader_program);                          // Bind shader program

	glActiveTexture(GL_TEXTURE0);                          // Bind texture
    glBindTexture(GL_TEXTURE_2D, in_texture_id);

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
		
	unbind_framebuffer();
}

// void Renderer::sizeChanged(const uint16_t width, const uint16_t height)
// {
// 	// tell opengl
// 	// glViewport(0, 0, width, height);
// }

void Renderer::update_triangle(int32_t viewport_w, int32_t viewport_h)
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

void Renderer::create_framebuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	uint16_t initial_width = 800;
	uint16_t initial_height = 600;

	glGenTextures(1, &out_texture_id);
	glBindTexture(GL_TEXTURE_2D, out_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, initial_width, initial_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out_texture_id, 0);

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

void Renderer::bind_framebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Renderer::unbind_framebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::rescale_framebuffer(float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, out_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out_texture_id, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

void Renderer::load_input_texture(const char* filename)
{
	glGenTextures(1, &in_texture_id);
    glBindTexture(GL_TEXTURE_2D, in_texture_id);

	// Set texture parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

void Renderer::refresh_screen()
{
	// flatten the 2D vector into a 1D vector
	std::vector<glm::vec3> flatArray;
    for (const auto& row : pixels) {
        flatArray.insert(flatArray.end(), row.begin(), row.end());
    }

	glBindTexture(GL_TEXTURE_2D, in_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_w, screen_h, 0, GL_RGB, GL_FLOAT, flatArray.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

//////////////////////////////////////////////////////////////////////////
// Drawing functions

void Renderer::draw_pixel(int32_t x, int32_t y, glm::vec3 color)
{
	// Check if the pixel is out of bounds
	if (x > 0 && x < screen_w && y > 0 && y < screen_h)
	{
		// Pixels are not 1x1 but pixel_w x pixel_h
		for (int32_t i = 0; i < pixel_w; i++)
		{
			for (int32_t j = 0; j < pixel_h; j++)
			{
				// cheeck if the pixel is out of bounds
				if (x + i > 0 && x + i < screen_w && y + j > 0 && y + j < screen_h)
					pixels[x + i][y + j] = color;
			}
		}
	}
}