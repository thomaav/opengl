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
#include "glwindow.h"
#include "material.h"

float cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
};

float widest = 1.0;

std::vector<glm::vec3> cube_positions = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, -1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
};

void update_cubes(bool increment)
{
	if (increment) {
		widest += 1.0f;
		cube_positions.push_back(glm::vec3(widest, 0.0f, widest));
		cube_positions.push_back(glm::vec3(-widest, 0.0f, -widest));
	} else {
		if (cube_positions.size() > 1) {
			widest -= 1.0f;
			cube_positions.pop_back();
			cube_positions.pop_back();
		}
	}
}

glm::vec3 light_color{1.0f, 1.0f, 1.0f};
glm::vec3 light_position{0.0f, 1.0f, 0.0f};

bool use_texture = 1;

int main(int argc, char *argv[])
{
	Window main_window{true};

	Shader texture_shader{"shaders/texture_vs.glsl", "shaders/texture_fs.glsl"};
	Shader lighting_shader{"shaders/lighting_vs.glsl", "shaders/lighting_fs.glsl"};
	Texture container_texture{"textures/container.jpg", false};
	Texture awesomeface_texture{"textures/awesomeface.png", true};
	Texture minecraft_texture{"textures/minecraft.png", false};

	GLuint default_VAO;
	glGenVertexArrays(1, &default_VAO);
	glBindVertexArray(default_VAO);

	GLuint lighting_VAO;
	glGenVertexArrays(1, &lighting_VAO);
	glBindVertexArray(lighting_VAO);

	//==== default VAO
	glBindVertexArray(default_VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	// texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal values
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	texture_shader.use();
	texture_shader.set_int("container_texture_sampler", 0);
	container_texture.use(GL_TEXTURE0);

	//==== lighting VAO
	glBindVertexArray(lighting_VAO);

	GLuint lighting_VBO;
	glGenBuffers(1, &lighting_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, lighting_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	//==== unbind for good measure
	glBindVertexArray(0);

	while (!main_window.should_close()) {
		main_window.process_input();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//==== default stuff
		texture_shader.use();
		texture_shader.set_mat4("view", main_window.view);
		texture_shader.set_mat4("projection", main_window.projection);

		glBindVertexArray(default_VAO);

		if (!use_texture) {
			texture_shader.set_vec3("material.ambient_lighting", emerald.ambient_lighting);
			texture_shader.set_vec3("material.diffuse_lighting", emerald.diffuse_lighting);
			texture_shader.set_vec3("material.specular_lighting", emerald.specular_lighting);
			texture_shader.set_float("material.shininess", emerald.shininess);
		} else {
			texture_shader.set_vec3("material.ambient_lighting", texture.ambient_lighting);
			texture_shader.set_vec3("material.diffuse_lighting", texture.diffuse_lighting);
			texture_shader.set_vec3("material.specular_lighting", texture.specular_lighting);
			texture_shader.set_float("material.shininess", texture.shininess);
		}

		texture_shader.set_bool("use_texture", use_texture);
		texture_shader.set_int("container_texture_sampler", 0);
		container_texture.use(GL_TEXTURE0);

		texture_shader.set_vec3("light_color", light_color);
		texture_shader.set_vec3("light_position", light_position);
		texture_shader.set_vec3("camera_position", main_window.get_camera_position());

		for (const auto position : cube_positions) {
			main_window.reset_model();
			main_window.translate_model(position);
			main_window.rotate_model((float) glfwGetTime(), 0.0f, 0.0f, 1.0f);
			main_window.scale_model(0.5f);
			texture_shader.set_mat4("model", main_window.model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//==== lighting
		glBindTexture(GL_TEXTURE_2D, 0);

		lighting_shader.use();
		lighting_shader.set_mat4("view", main_window.view);
		lighting_shader.set_mat4("projection", main_window.projection);

		glBindVertexArray(lighting_VAO);

		lighting_shader.set_vec3("light_color", light_color);
		lighting_shader.set_vec3("object_color", glm::vec3{1.0f, 1.0f, 1.0f});

		main_window.reset_model();
		main_window.translate_model(light_position);
		main_window.scale_model(0.05f);
		lighting_shader.set_mat4("model", main_window.model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// poll current events and swap the buffers
		glfwPollEvents();
		main_window.swap_buffers();
	}

	glDeleteVertexArrays(1, &default_VAO);
	glDeleteBuffers(1, &VBO);
	// glDeleteBuffers(1, &EBO);

	glfwTerminate();
}
