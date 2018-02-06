#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glcamera.h"

class Window {
public:
	const int width = 800;
	const int height = 600;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	Window();
	~Window();

	void process_input();
	bool should_close();
	void swap_buffers();
private:
	Camera camera;
	GLFWwindow *window;
};
