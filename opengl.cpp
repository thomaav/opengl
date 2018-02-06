#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glshader.h"
#include "gltexture.h"
#include "window.h"

float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

std::vector<glm::vec3> cube_positions = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, -1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
};

int main(int argc, char *argv[])
{
	Window main_window{};

	Shader default_shader{"shaders/vertexshader.glsl", "shaders/fragmentshader.glsl"};
	Texture container_texture{"textures/container.jpg", false};
	Texture awesomeface_texture{"textures/awesomeface.png", true};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	// texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	default_shader.use();
	default_shader.set_int("container_texture_sampler", 0);
	default_shader.set_int("awesomeface_texture_sampler", 1);
	container_texture.use(GL_TEXTURE0);
	awesomeface_texture.use(GL_TEXTURE1);

	// unbind for good measure
	glBindVertexArray(0);

	while (!main_window.should_close()) {
		main_window.process_input();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);

		default_shader.set_mat4("view", main_window.view);
		default_shader.set_mat4("projection", main_window.projection);

		for (const auto position : cube_positions) {
			main_window.model = glm::mat4{};
			main_window.model = glm::translate(main_window.model, position);
			main_window.model = glm::rotate(main_window.model, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
			default_shader.set_mat4("model", main_window.model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// poll current events and swap the buffers
		glfwPollEvents();
		main_window.swap_buffers();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// glDeleteBuffers(1, &EBO);

	glfwTerminate();
}
