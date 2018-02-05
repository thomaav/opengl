#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glshader.h"
#include "gltexture.h"
#include "window.h"

constexpr int width = 800;
constexpr int height = 600;

float mouse_speed = 0.001f;
float speed = 0.05f;
float horizontal_angle = 3.14f;
float vertical_angle = 0.0f;
float last_time;

glm::vec3 direction(cos(vertical_angle) * sin(horizontal_angle),
		    sin(vertical_angle),
		    cos(vertical_angle) * cos(horizontal_angle));
glm::vec3 right(sin(horizontal_angle - 3.14f / 2.0f), 0,
		cos(horizontal_angle - 3.14f / 2.0f));
glm::vec3 up = glm::cross(right, direction);
glm::vec3 position(0.0f, 0.0f, 3.0f);

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

const float rectangle[] = {
	0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

const unsigned indices[] = {
	0, 1, 2,
	0, 2, 3,
};

int main(int argc, char *argv[])
{
	GLFWwindow *main_window = init_main_window(width, height);

	Shader default_shader{"shaders/vertexshader.glsl", "shaders/fragmentshader.glsl"};

	Texture container_texture{"textures/container.jpg", false};
	Texture awesomeface_texture{"textures/awesomeface.png", true};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// ==== buffers for basic rectangle object
	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	// colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// ====

	default_shader.use();
	default_shader.set_int("container_texture_sampler", 0);
	default_shader.set_int("awesomeface_texture_sampler", 1);
	container_texture.use(GL_TEXTURE0);
	awesomeface_texture.use(GL_TEXTURE1);

	glm::mat4 trans;

	// let's go 3D
	model = glm::rotate(glm::mat4{}, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(glm::mat4{}, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height,
						0.1f, 100.0f);

	// unbind for good measure
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(main_window)) {
		// process all input
		process_input(main_window);

		// rendering work
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);

		default_shader.set_mat4("model", model);
		default_shader.set_mat4("view", view);
		default_shader.set_mat4("projection", projection);

		trans = glm::mat4{};
		trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));
		default_shader.set_mat4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = glm::mat4{};
		trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
		trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));
		default_shader.set_mat4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// poll current events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(main_window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}
