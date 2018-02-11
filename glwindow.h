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
	const int width = 1920;
	const int height = 1200;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	Window(bool fullscreen = true);
	~Window();

	void process_input();
	void update_fov(double yoffset);
	void update_lighting(char color, double yoffset);
	bool should_close();
	void swap_buffers();

	void reset_model();

	void translate_model(float x, float y, float z);
	void translate_model(glm::vec3 translation);

	void rotate_model(float radians, float x, float y, float z);
	void rotate_model(float radians, glm::vec3 rotation);

	void scale_model(float x, float y, float z);
	void scale_model(float scalar);

	void toggle_mouse();
	glm::vec3 get_camera_position();
private:
	float fov;
	const float max_fov = 60.0f;
	const float min_fov = 1.0f;

	double last_frame_time = 0.0;
	int nframes = 0;

	Camera camera;
	GLFWwindow *window;
};
