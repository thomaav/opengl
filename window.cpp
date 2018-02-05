#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"

extern glm::mat4 model;
extern glm::mat4 view;
extern glm::mat4 projection;

GLFWwindow *init_main_window(const unsigned width, const unsigned height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "~", NULL, NULL);

	if (!window) {
		std::cout << "GLFW window creation failed; terminating" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, fb_resize_cb);

	return window;
}

void fb_resize_cb(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		view = glm::translate(glm::mat4{}, glm::vec3(0.0f, 0.0f, 0.05f)) * view;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		view = glm::translate(glm::mat4{}, glm::vec3(0.05f, 0.0f, 0.0f)) * view;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		view = glm::translate(glm::mat4{}, glm::vec3(0.0f, 0.0f, -0.05f)) * view;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		view = glm::translate(glm::mat4{}, glm::vec3(-0.05f, 0.0f, 0.0f)) * view;
}
