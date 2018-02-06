#include <iostream>
#include <cmath>

#include "glwindow.h"

namespace {
	void fb_resize_cb(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void scroll_cb(GLFWwindow *window, double xoffset, double yoffset)
	{
		Window *owner = (Window *) glfwGetWindowUserPointer(window);
		owner->update_fov(yoffset);
	}
}

Window::Window()
	: fov(45.0f)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "~", NULL, NULL);

	if (!window) {
		std::cout << "GLFW window creation failed; terminating" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glViewport(0, 0, width, height);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, fb_resize_cb);
	glfwSetScrollCallback(window, scroll_cb);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glEnable(GL_DEPTH_TEST);

	// initialize 3D world settings
	view = glm::translate(glm::mat4{}, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(fov), (float) width / (float) height, 0.1f, 100.0f);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

void Window::process_input()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	camera.update_direction(window);
	camera.update_position(window);
	view = camera.view_mat4();
}

void Window::update_fov(double yoffset)
{
	if (fov >= min_fov && fov <= max_fov)
		fov -= yoffset;

	fov = fmax(min_fov, fov);
	fov = fmin(max_fov, fov);

	projection = glm::perspective(glm::radians(fov), (float) width / (float) height, 0.1f, 100.0f);
}

bool Window::should_close()
{
	return glfwWindowShouldClose(window);
}

void Window::swap_buffers()
{
	glfwSwapBuffers(window);
}
