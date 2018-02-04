#include <iostream>

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

int main(int argc, char *argv[])
{
	GLFWwindow *main_window = init_simple_window();
	glfwMakeContextCurrent(main_window);
	init_glad();

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(main_window, fb_resize_cb);

	// unsigned VBO;
	// glGenBuffers(1, &VBO);

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
