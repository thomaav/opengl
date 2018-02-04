#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

GLFWwindow *init_simple_window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "~", NULL, NULL);

	if (!window) {
		std::cout << "GLFW window creation failed; terminating" << std::endl;
		glfwTerminate();
		exit(1);
	}

	return window;
}

void init_glad()
{
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

void fb_resize_cb(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,
};

bool load_shader_src(const std::string &filename, std::string &buf)
{
	std::ifstream f;
	f.open(filename.c_str());

	if (!f)
		return false;

	std::stringstream stream;
	stream << f.rdbuf();
	buf = stream.str();

	f.close();
	return true;
}

void shader_compilation_status(GLuint &shader)
{
	int success; char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	}
}

void program_link_status(GLuint &program)
{
	int success; char info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}
}

int main(int argc, char *argv[])
{
	GLFWwindow *main_window = init_simple_window();
	glfwMakeContextCurrent(main_window);
	init_glad();

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(main_window, fb_resize_cb);

	// ==== buffer for basic triangle object
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ====

	// ==== vertex shader
	std::string vertex_shader_src;
	load_shader_src("shaders/vertexshader.glsl", vertex_shader_src);
	const char *vsc_c_str = vertex_shader_src.c_str();

	GLuint vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vsc_c_str, NULL);
	glCompileShader(vertex_shader);
	shader_compilation_status(vertex_shader);
	// ====

	// ==== fragment shader
	std::string fragment_shader_src;
	load_shader_src("shaders/fragmentshader.glsl", fragment_shader_src);
	const char *fsc_c_str = fragment_shader_src.c_str();

	GLuint fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fsc_c_str, NULL);
	glCompileShader(fragment_shader);
	shader_compilation_status(fragment_shader);
	// ====

	// ==== shader program
	GLuint shader_program;
	shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);
	program_link_status(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glUseProgram(shader_program);
	// ====

	while (!glfwWindowShouldClose(main_window)) {
		// process all input
		process_input(main_window);

		// rendering work
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// poll current events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(main_window);
	}

	glfwTerminate();
}
