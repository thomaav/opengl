#include "glwindow.h"
#include "glcamera.h"

Camera::Camera()
	: mouse_speed(0.010f)
	, speed(2.0f)
	, horizontal_angle(3.14f)
	, vertical_angle(0.0f)
	, last_time(glfwGetTime())
{
	direction.x = cos(vertical_angle) * sin(horizontal_angle);
	direction.y = sin(vertical_angle);
	direction.z = cos(vertical_angle) * cos(horizontal_angle);

	right.x = sin(horizontal_angle - 3.14f / 2.0f);
	right.y = 0;
	right.z = cos(horizontal_angle - 3.14f / 2.0f);

	up = glm::cross(right, direction);

	position = glm::vec3{0.0f, 1.0f, 3.0f};
}

Camera::~Camera()
{
	;
}

void Camera::update(GLFWwindow *window)
{
	update_direction(window);
	update_position(window);
	last_time = glfwGetTime();
}

void Camera::update_direction(GLFWwindow *window)
{
	int width, height;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	float current_time = glfwGetTime();
	float delta_time = current_time - last_time;

	horizontal_angle += mouse_speed * delta_time * float(width / 2 - x);
	vertical_angle += mouse_speed * delta_time * float(height / 2 - y);

	if (vertical_angle > 1.57)
		vertical_angle = 1.57;
	if (vertical_angle < -1.57)
		vertical_angle = -1.57;

	direction.x = cos(vertical_angle) * sin(horizontal_angle);
	direction.y = sin(vertical_angle);
	direction.z = cos(vertical_angle) * cos(horizontal_angle);

	right.x = sin(horizontal_angle - 3.14f / 2.0f);
	right.y = 0;
	right.z = cos(horizontal_angle - 3.14f / 2.0f);

	up = glm::cross(right, direction);
}

void Camera::update_position(GLFWwindow *window)
{
	Window *owner = (Window *) glfwGetWindowUserPointer(window);
	float current_time = glfwGetTime();
	float delta_time = current_time - last_time;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 6.0f;
	else
		speed = 2.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += direction * delta_time * speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= direction * delta_time * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += right * delta_time * speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= right * delta_time * speed;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		owner->update_lighting(1.0f);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		owner->update_lighting(-1.0f);
}

glm::mat4 Camera::view_mat4()
{
	return glm::lookAt(position, position + direction, up);
}

glm::vec3 Camera::get_position()
{
	return position;
}
