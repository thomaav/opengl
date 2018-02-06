#include <iostream>
#include <cmath>

#include "glwindow.h"

void update_cubes(bool increment);

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

	void click_cb(GLFWwindow *window, int button, int action, int mods)
	{
		if (!(action == GLFW_PRESS))
			return;

		if (button == GLFW_MOUSE_BUTTON_1)
			update_cubes(true);
		else if (button == GLFW_MOUSE_BUTTON_2)
			update_cubes(false);
	}
}

Window::Window(bool fullscreen)
	: fov(45.0f)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor *screen = fullscreen ? glfwGetPrimaryMonitor() : NULL;
	window = glfwCreateWindow(width, height, "~", screen, NULL);

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
	glfwSetMouseButtonCallback(window, click_cb);
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

	camera.update(window);
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

void Window::reset_model()
{
	model = glm::mat4{};
}

void Window::translate_model(float x, float y, float z)
{
	model = glm::translate(model, glm::vec3{x, y, z});
}

void Window::translate_model(glm::vec3 translation)
{
	model = glm::translate(model, translation);
}

void Window::rotate_model(float radians, float x, float y, float z)
{
	model = glm::rotate(model, radians, glm::vec3{x, y, z});
}

void Window::rotate_model(float radians, glm::vec3 rotation)
{
	model = glm::rotate(model, radians, rotation);
}

void Window::scale_model(float x, float y, float z)
{
	model = glm::scale(model, glm::vec3{x, y, z});
}

void Window::scale_model(float scalar)
{
	model = glm::scale(model, glm::vec3(scalar, scalar, scalar));
}
