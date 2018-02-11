#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera();
	~Camera();
	void update(GLFWwindow *window);
	void update_direction(GLFWwindow *window);
	void toggle_mouse();
	void update_position(GLFWwindow *window);
	glm::mat4 view_mat4();
	glm::vec3 get_position();
private:
	bool init_scene;
	bool mouse_enabled;
	float mouse_speed;
	float speed;
	float horizontal_angle;
	float vertical_angle;
	float last_time;

	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 position;
};
