#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>

#include "glshader.h"
#include "window.h"

const float first_triangle[] = {
	0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f, // top right
	0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // bottom left
};

const float second_triangle[] = {
	-0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f, // top left
	0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f, // top right
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // bottom left
};

int main(int argc, char *argv[])
{
	GLFWwindow *main_window = init_main_window(800, 600);

	// ==== shaders
	Shader default_shader{"shaders/vertexshader.glsl", "shaders/fragmentshader.glsl"};
	// ====

	// ==== vertex array objects
	GLuint VAOs[2];
	glGenVertexArrays(2, VAOs);
	// ====

	// ==== buffers for basic triangle objects
	GLuint VBOs[2];
	glGenBuffers(2, VBOs);

	// store first triangle in a VAO
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// store second triangle in a VAO
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ====

	// unbind for good measure
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(main_window)) {
		// process all input
		process_input(main_window);

		// rendering work
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// float time = glfwGetTime();
		// float color_value = sin(time) / 2.0f + 0.5f;
		// GLuint vertex_color_location = glGetUniformLocation(default_shader.get_program(), "cpu_color");

		default_shader.use();

		// glUniform4f(vertex_color_location, 0.0f, color_value, 0.0f, 1.0f);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// poll current events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(main_window);
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
}
