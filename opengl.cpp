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
	GLFWwindow *main_window = init_main_window(800, 600);

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

	// unbind for good measure
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(main_window)) {
		// process all input
		process_input(main_window);

		// rendering work
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);

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
