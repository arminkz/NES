#include "Renderer.h"

#include "Window.h"

/// [Protected methods]

Renderer::Renderer()
{}

/// [Public methods]

Renderer::~Renderer()
{
	//TODO: shouldn't do anything if the class is not initialized.
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &texture_id);
	glDeleteRenderbuffers(1, &RBO);
}

void Renderer::initialize(GLFWwindow* window)
{
	this->_window = window;

	create_triangle();
	create_shaders();
	create_framebuffer();
}

//Used to draw renderables inside one ImGui window
void Renderer::render(uint16_t width, uint16_t height)
{
	rescale_framebuffer(width, height);
	glViewport(0, 0, width, height);

	ImVec2 pos = ImGui::GetCursorScreenPos();
		
	ImGui::GetWindowDrawList()->AddImage(
		(ImTextureID)(intptr_t)texture_id, 
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + width, pos.y + height), 
		ImVec2(0, 1), 
		ImVec2(1, 0)
	);

	bind_framebuffer();
		
	glUseProgram(shader_program);     // Bind shader program
	glBindVertexArray(VAO);           // Bind VAO
	glDrawArrays(GL_TRIANGLES, 0, 3); // Draw triangle
	glBindVertexArray(0);             // Unbind VAO
	glUseProgram(0);                  // Unbind shader program
		
	unbind_framebuffer();
}

// void Renderer::sizeChanged(const uint16_t width, const uint16_t height)
// {
// 	// tell opengl
// 	// glViewport(0, 0, width, height);
// }


void Renderer::create_triangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, // 1. vertex x, y, z
		1.0f, -1.0f, 0.0f,  // 2. vertex ...
		0.0f, 1.0f, 0.0f    // etc... 
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // Bind VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Define a vertex attribute pointer
	glEnableVertexAttribArray(0); // Enable the vertex attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
	glBindVertexArray(0); // Unbind VAO
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
	layout (location = 0) in vec3 pos;
	void main()
	{
		gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	}
	)*";

	const char* fragment_shader_code = R"*(
	#version 330
	out vec4 color;
	void main()
	{
		color = vec4(0.0, 1.0, 0.0, 1.0);
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
}

void Renderer::create_framebuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	uint16_t initial_width = 800;
	uint16_t initial_height = 600;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, initial_width, initial_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

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
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}
