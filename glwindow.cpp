#include <iostream>
#include <cmath>

#include "glwindow.h"
#include "gllight.h"
#include "glmodel.h"

extern Model *nanosuit_ptr;

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
		Window *owner = (Window *) glfwGetWindowUserPointer(window);

		if (!(action == GLFW_PRESS))
			return;

		if (button == GLFW_MOUSE_BUTTON_2)
			owner->toggle_mouse();
	}

	void key_cb(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS && key == GLFW_KEY_I) {
			nanosuit_ptr->rigid_body->applyImpulse(btVector3(0.0f, 0.0f, 1.0f),
							       btVector3(0.0f, 0.0f, 0.0f));
		}

		if (action == GLFW_PRESS && key == GLFW_KEY_K) {
			nanosuit_ptr->rigid_body->applyImpulse(btVector3(0.0f, 0.0f, -1.0f),
							       btVector3(0.0f, 0.0f, 0.0f));
		}

		if (action == GLFW_PRESS && key == GLFW_KEY_J) {
			nanosuit_ptr->rigid_body->applyImpulse(btVector3(1.0f, 0.0f, 0.0f),
							       btVector3(0.0f, 0.0f, 0.0f));
		}

		if (action == GLFW_PRESS && key == GLFW_KEY_L) {
			nanosuit_ptr->rigid_body->applyImpulse(btVector3(-1.0f, 0.0f, 0.0f),
							       btVector3(0.0f, 0.0f, 0.0f));
		}

		if (action == GLFW_PRESS && key == GLFW_KEY_ENTER) {
			nanosuit_ptr->rigid_body->applyImpulse(btVector3(0.0f, 3.0f, 0.0f),
							       btVector3(0.0f, 0.0f, 0.0f));
		}
	}

	void error_cb(int error, const char *description)
	{
		std::cout << "Error: " << description << std::endl;
	}

	void change_lighting_dynamically(GLFWwindow *window)
	{
		Window *owner = (Window *) glfwGetWindowUserPointer(window);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			owner->update_lighting(0.0f, 1.0f);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			owner->update_lighting(0.0f, -1.0f);

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			point_light->position.x += 0.05;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			point_light->position.x -= 0.05;

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
	glfwSetErrorCallback(error_cb);

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
	glfwSwapInterval(1);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glViewport(0, 0, width, height);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, fb_resize_cb);
	glfwSetMouseButtonCallback(window, click_cb);
	glfwSetKeyCallback(window, key_cb);
	glfwSetScrollCallback(window, scroll_cb);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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
		point_light->ambient.x += yoffset * 0.05f;
		point_light->ambient.y += yoffset * 0.05f;
		point_light->ambient.z += yoffset * 0.05f;
		point_light->diffuse.x += yoffset * 0.05f;
		point_light->diffuse.y += yoffset * 0.05f;
		point_light->diffuse.z += yoffset * 0.05f;
		point_light->specular.x += yoffset * 0.05f;
		point_light->specular.y += yoffset * 0.05f;
		point_light->specular.z += yoffset * 0.05f;
	}

	if (color) {
		switch (color) {
		case 'r':
			point_light->ambient.x += 0.05f;
			point_light->diffuse.x += 0.05f;
			point_light->specular.x += 0.05f;
			break;
		case 'g':
			point_light->ambient.y += 0.05f;
			point_light->diffuse.y += 0.05f;
			point_light->specular.y += 0.05f;
			break;
		case 'b':
			point_light->ambient.z += 0.05f;
			point_light->diffuse.z += 0.05f;
			point_light->specular.z += 0.05f;
			break;
		case 'x':
			point_light->ambient.x =
				point_light->ambient.y =
				point_light->ambient.z = 0.0f;
			point_light->diffuse.x =
				point_light->diffuse.y =
				point_light->diffuse.z = 0.0f;
			point_light->specular.x =
				point_light->specular.y =
				point_light->specular.z = 0.0f;
			break;
		default:
			break;
		}
	}

	point_light->ambient.x = fmax(0.0f, point_light->ambient.x);
	point_light->ambient.x = fmin(1.0f, point_light->ambient.x);
	point_light->ambient.y = fmax(0.0f, point_light->ambient.y);
	point_light->ambient.y = fmin(1.0f, point_light->ambient.y);
	point_light->ambient.z = fmax(0.0f, point_light->ambient.z);
	point_light->ambient.z = fmin(1.0f, point_light->ambient.z);

	point_light->diffuse.x = fmax(0.0f, point_light->diffuse.x);
	point_light->diffuse.x = fmin(1.0f, point_light->diffuse.x);
	point_light->diffuse.y = fmax(0.0f, point_light->diffuse.y);
	point_light->diffuse.y = fmin(1.0f, point_light->diffuse.y);
	point_light->diffuse.z = fmax(0.0f, point_light->diffuse.z);
	point_light->diffuse.z = fmin(1.0f, point_light->diffuse.z);

	point_light->specular.x = fmax(0.0f, point_light->specular.x);
	point_light->specular.x = fmin(1.0f, point_light->specular.x);
	point_light->specular.y = fmax(0.0f, point_light->specular.y);
	point_light->specular.y = fmin(1.0f, point_light->specular.y);
	point_light->specular.z = fmax(0.0f, point_light->specular.z);
	point_light->specular.z = fmin(1.0f, point_light->specular.z);
}

bool Window::should_close()
{
	return glfwWindowShouldClose(window);
}

void Window::swap_buffers()
{
	double current_frame_time = glfwGetTime();
	++nframes;

	if (current_frame_time - last_frame_time >= 1.0) {
		// std::cout << 1000.0 / double(nframes) << " ms/frame" << std::endl;
		nframes = 0;
		last_frame_time += 1.0;
	}

	glfwSwapBuffers(window);
}

void Window::toggle_mouse()
{
	camera.toggle_mouse();
}

glm::vec3 Window::get_camera_position()
{
	return camera.get_position();
}

glm::vec3 Window::get_camera_direction()
{
	return camera.get_direction();
}
