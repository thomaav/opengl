#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"

extern float mouse_speed;
extern float speed;
extern float horizontal_angle;
extern float vertical_angle;
extern float last_time;

extern glm::vec3 direction;
extern glm::vec3 right;
extern glm::vec3 up;
extern glm::vec3 position;

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glEnable(GL_DEPTH_TEST);

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

	int width, height;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	float current_time = glfwGetTime();
	float delta_time = current_time - last_time;

	horizontal_angle += mouse_speed * delta_time * float(width / 2 - x);
	vertical_angle += mouse_speed * delta_time * float(height / 2 - y);

	direction = glm::vec3(cos(vertical_angle) * sin(horizontal_angle),
			      sin(vertical_angle),
			      cos(vertical_angle) * cos(horizontal_angle));
	right = glm::vec3(sin(horizontal_angle - 3.14f / 2.0f), 0,
			  cos(horizontal_angle - 3.14f / 2.0f));
	up = glm::cross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += direction * delta_time * speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= direction * delta_time * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += right * delta_time * speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= right * delta_time * speed;

	view = glm::lookAt(position, position + direction, up);
}
