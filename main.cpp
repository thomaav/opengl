#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>

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

const float first_triangle[] = {
	0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f, // top right
	0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // bottom left
};

const float second_triangle[] = {
	-0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f, // top left
	0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f, // top right
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // bottom left
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
	// ====

	// ==== vertex array objects
	GLuint VAOs[2];
	glGenVertexArrays(2, VAOs);
	// ====

	// ==== buffers for basic triangle objects
	GLuint VBOs[2];
	glGenBuffers(2, VBOs);

	// store first triangle in a VAO
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// store second triangle in a VAO
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ====

	// unbind for good measure
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(main_window)) {
		// process all input
		process_input(main_window);

		// rendering work
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// change color of triangle using uniform in shaders,
		// scale sin to only allow for [0.0f, 1.0f]
		float time = glfwGetTime();
		float color_value = sin(time) / 2.0f + 0.5f;
		GLuint vertex_color_location = glGetUniformLocation(shader_program, "cpu_color");

		glUseProgram(shader_program);

		glUniform4f(vertex_color_location, 0.0f, color_value, 0.0f, 1.0f);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// poll current events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(main_window);
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
}
