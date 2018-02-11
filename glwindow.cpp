#include <iostream>
#include <cmath>

#include "glwindow.h"
#include "gllight.h"

void update_cubes(bool increment);

namespace {
	void fb_resize_cb(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void scroll_cb(GLFWwindow *window, double xoffset, double yoffset)
	{
		Window *owner = (Window *) glfwGetWindowUserPointer(window);
		owner->update_lighting(0.0f, yoffset);
	}

	void click_cb(GLFWwindow *window, int button, int action, int mods)
	{
		if (!(action == GLFW_PRESS))
			return;
	}

	void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
	}

	void change_lighting_dynamically(GLFWwindow *window)
	{
		Window *owner = (Window *) glfwGetWindowUserPointer(window);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			owner->update_lighting(0.0f, 1.0f);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			owner->update_lighting(0.0f, -1.0f);

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			light.position.x += 0.05;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			light.position.x -= 0.05;

		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			owner->update_lighting('r', 0.0f);
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
			owner->update_lighting('g', 0.0f);
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
			owner->update_lighting('b', 0.0f);
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			owner->update_lighting('x', 0.0f);
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
	glfwSetKeyCallback(window, key_cb);
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

	change_lighting_dynamically(window);
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

void Window::update_lighting(char color, double yoffset)
{
	if (yoffset) {
		light.ambient.x += yoffset * 0.05f;
		light.ambient.y += yoffset * 0.05f;
		light.ambient.z += yoffset * 0.05f;
		light.diffuse.x += yoffset * 0.05f;
		light.diffuse.y += yoffset * 0.05f;
		light.diffuse.z += yoffset * 0.05f;
		light.specular.x += yoffset * 0.05f;
		light.specular.y += yoffset * 0.05f;
		light.specular.z += yoffset * 0.05f;
	}

	if (color) {
		switch (color) {
		case 'r':
			light.ambient.x += 0.05f;
			light.diffuse.x += 0.05f;
			light.specular.x += 0.05f;
			break;
		case 'g':
			light.ambient.y += 0.05f;
			light.diffuse.y += 0.05f;
			light.specular.y += 0.05f;
			break;
		case 'b':
			light.ambient.z += 0.05f;
			light.diffuse.z += 0.05f;
			light.specular.z += 0.05f;
			break;
		case 'x':
			light.ambient.x = light.ambient.y = light.ambient.z = 0.0f;
			light.diffuse.x = light.diffuse.y = light.diffuse.z = 0.0f;
			light.specular.x = light.specular.y = light.specular.z = 0.0f;
			break;
		default:
			break;
		}
	}

	light.ambient.x = fmax(0.0f, light.ambient.x);
	light.ambient.x = fmin(1.0f, light.ambient.x);
	light.ambient.y = fmax(0.0f, light.ambient.y);
	light.ambient.y = fmin(1.0f, light.ambient.y);
	light.ambient.z = fmax(0.0f, light.ambient.z);
	light.ambient.z = fmin(1.0f, light.ambient.z);

	light.diffuse.x = fmax(0.0f, light.diffuse.x);
	light.diffuse.x = fmin(1.0f, light.diffuse.x);
	light.diffuse.y = fmax(0.0f, light.diffuse.y);
	light.diffuse.y = fmin(1.0f, light.diffuse.y);
	light.diffuse.z = fmax(0.0f, light.diffuse.z);
	light.diffuse.z = fmin(1.0f, light.diffuse.z);

	light.specular.x = fmax(0.0f, light.specular.x);
	light.specular.x = fmin(1.0f, light.specular.x);
	light.specular.y = fmax(0.0f, light.specular.y);
	light.specular.y = fmin(1.0f, light.specular.y);
	light.specular.z = fmax(0.0f, light.specular.z);
	light.specular.z = fmin(1.0f, light.specular.z);
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

glm::vec3 Window::get_camera_position()
{
	return camera.get_position();
}
