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
#include "glmaterial.h"
#include "gllight.h"
#include "glmesh.h"
#include "glmodel.h"

std::vector<Vertex> cube_vertices = {
	{ glm::vec3{-0.5f, -0.5f, -0.5f},   glm::vec3{0.0f,  0.0f, -1.0f},    glm::vec2{0.0f, 0.0f} },
	{ glm::vec3{0.5f, -0.5f, -0.5f},    glm::vec3{ 0.0f,  0.0f, -1.0f},   glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{0.5f,  0.5f, -0.5f},    glm::vec3{ 0.0f,  0.0f, -1.0f},   glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{0.5f,  0.5f, -0.5f},    glm::vec3{ 0.0f,  0.0f, -1.0f},   glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{-0.5f,  0.5f, -0.5f},   glm::vec3{0.0f,  0.0f, -1.0f},    glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{-0.5f, -0.5f, -0.5f},   glm::vec3{0.0f,  0.0f, -1.0f},    glm::vec2{0.0f, 0.0f} },

	{ glm::vec3{-0.5f, -0.5f,  0.5f},   glm::vec3{0.0f,  0.0f, 1.0f},     glm::vec2{0.0f, 0.0f} },
	{ glm::vec3{0.5f, -0.5f,  0.5f},    glm::vec3{ 0.0f,  0.0f, 1.0f},    glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{0.5f,  0.5f,  0.5f},    glm::vec3{ 0.0f,  0.0f, 1.0f},    glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{0.5f,  0.5f,  0.5f},    glm::vec3{ 0.0f,  0.0f, 1.0f},    glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{-0.5f,  0.5f,  0.5f},   glm::vec3{0.0f,  0.0f, 1.0f},     glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{-0.5f, -0.5f,  0.5f},   glm::vec3{0.0f,  0.0f, 1.0f},     glm::vec2{0.0f, 0.0f} },

	{ glm::vec3{-0.5f,  0.5f,  0.5f},   glm::vec3{-1.0f,  0.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{-0.5f,  0.5f, -0.5f},   glm::vec3{-1.0f,  0.0f,  0.0f},   glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{-0.5f, -0.5f, -0.5f},   glm::vec3{-1.0f,  0.0f,  0.0f},   glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{-0.5f, -0.5f, -0.5f},   glm::vec3{-1.0f,  0.0f,  0.0f},   glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{-0.5f, -0.5f,  0.5f},   glm::vec3{-1.0f,  0.0f,  0.0f},   glm::vec2{0.0f, 0.0f} },
	{ glm::vec3{-0.5f,  0.5f,  0.5f},   glm::vec3{-1.0f,  0.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },

	{ glm::vec3{0.5f,  0.5f,  0.5f},    glm::vec3{ 1.0f,  0.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{0.5f,  0.5f, -0.5f},    glm::vec3{ 1.0f,  0.0f,  0.0f},   glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{0.5f, -0.5f, -0.5f},    glm::vec3{ 1.0f,  0.0f,  0.0f},   glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{0.5f, -0.5f, -0.5f},    glm::vec3{ 1.0f,  0.0f,  0.0f},   glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{0.5f, -0.5f,  0.5f},    glm::vec3{ 1.0f,  0.0f,  0.0f},   glm::vec2{0.0f, 0.0f} },
	{ glm::vec3{0.5f,  0.5f,  0.5f},    glm::vec3{ 1.0f,  0.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },

	{ glm::vec3{-0.5f, -0.5f, -0.5f},   glm::vec3{0.0f, -1.0f,  0.0f},    glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{0.5f, -0.5f, -0.5f},    glm::vec3{ 0.0f, -1.0f,  0.0f},   glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{0.5f, -0.5f,  0.5f},    glm::vec3{ 0.0f, -1.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{0.5f, -0.5f,  0.5f},    glm::vec3{ 0.0f, -1.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{-0.5f, -0.5f,  0.5f},   glm::vec3{0.0f, -1.0f,  0.0f},    glm::vec2{0.0f, 0.0f} },
	{ glm::vec3{-0.5f, -0.5f, -0.5f},   glm::vec3{0.0f, -1.0f,  0.0f},    glm::vec2{0.0f, 1.0f} },

	{ glm::vec3{-0.5f,  0.5f, -0.5f},   glm::vec3{0.0f,  1.0f,  0.0f},    glm::vec2{0.0f, 1.0f} },
	{ glm::vec3{0.5f,  0.5f, -0.5f},    glm::vec3{ 0.0f,  1.0f,  0.0f},   glm::vec2{1.0f, 1.0f} },
	{ glm::vec3{0.5f,  0.5f,  0.5f},    glm::vec3{ 0.0f,  1.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{0.5f,  0.5f,  0.5f},    glm::vec3{ 0.0f,  1.0f,  0.0f},   glm::vec2{1.0f, 0.0f} },
	{ glm::vec3{-0.5f,  0.5f,  0.5f},   glm::vec3{0.0f,  1.0f,  0.0f},    glm::vec2{0.0f, 0.0f} },
	{ glm::vec3{-0.5f,  0.5f, -0.5f},   glm::vec3{0.0f, 1.0f, 0.0f},      glm::vec2{0.0f, 1.0f} },
};

std::vector<GLuint> cube_indices = {
	0, 1, 2,
	3, 4, 5,

	6, 7, 8,
	9, 10, 11,

	12, 13, 14,
	15, 16, 17,

	18, 19, 20,
	21, 22, 23,

	24, 25, 26,
	27, 28, 29,

	30, 31, 32,
	33, 34, 35,
};

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
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
};

float widest = 1.0;

std::vector<glm::vec3> cube_positions = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, -1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
};

float ground[] = {
	-1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,   30.0f, 0.0f,
	1.0f, 0.0f, -1.0f,  30.0f, 30.0f,

	-1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	-1.0f, 0.0f, -1.0f, 0.0f, 30.0f,
	1.0f, 0.0f, -1.0f,  30.0f, 30.0f,
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

int main(int argc, char *argv[])
{
	Window main_window{true};

	Shader texture_shader{"shaders/texture_vs.glsl", "shaders/texture_fs.glsl"};
	Shader lighting_shader{"shaders/lighting_vs.glsl", "shaders/lighting_fs.glsl"};
	Shader ground_shader{"shaders/ground_vs.glsl", "shaders/ground_fs.glsl"};
	Shader mesh_shader{"shaders/mesh_vs.glsl", "shaders/mesh_fs.glsl"};
	Shader model_shader{"shaders/model_vs.glsl", "shaders/model_fs.glsl"};
	Texture container_diffuse{"textures/container2.png", true};
	Texture container_specular{"textures/container2_specular.png", true};
	Texture container_emission{"textures/container2_emission.jpg", false};
	Texture awesomeface_texture{"textures/awesomeface.png", true};
	Texture minecraft_texture{"textures/minecraft.png", false};
	Texture ground_texture{"textures/grass.png", false};

	std::vector<Texture> cube_textures;

	Texture mesh_cube_texture_diffuse{"textures/container2.png", true};
	mesh_cube_texture_diffuse.type = "texture_diffuse";
	cube_textures.push_back(std::move(mesh_cube_texture_diffuse));

	Texture mesh_cube_texture_specular{"textures/container2_specular.png", true};
	mesh_cube_texture_specular.type = "texture_specular";
	cube_textures.push_back(std::move(mesh_cube_texture_specular));

	Texture mesh_cube_texture_emission{"textures/container2_emission.jpg", false};
	mesh_cube_texture_emission.type = "texture_diffuse";
	cube_textures.push_back(std::move(mesh_cube_texture_emission));

	Mesh cube_mesh{cube_vertices, cube_indices, std::move(cube_textures)};

	Model nanosuit{"models/nanosuit/nanosuit.obj"};

	GLuint default_VAO;
	glGenVertexArrays(1, &default_VAO);

	GLuint lighting_VAO;
	glGenVertexArrays(1, &lighting_VAO);

	GLuint ground_VAO;
	glGenVertexArrays(1, &ground_VAO);

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

	//==== lighting VAO
	glBindVertexArray(lighting_VAO);

	GLuint lighting_VBO;
	glGenBuffers(1, &lighting_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, lighting_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	//==== ground VAO
	glBindVertexArray(ground_VAO);

	GLuint ground_VBO;
	glGenBuffers(1, &ground_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, ground_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

		texture_shader.set_vec3("material.specular_lighting", steel_container.specular_lighting);
		texture_shader.set_float("material.shininess", steel_container.shininess);

		texture_shader.set_int("material.diffuse_lighting", 0);
		container_diffuse.use(GL_TEXTURE0);

		texture_shader.set_int("material.specular_lighting", 1);
		container_specular.use(GL_TEXTURE1);

		texture_shader.set_int("material.emission_lighting", 2);
		container_emission.use(GL_TEXTURE2);

		texture_shader.set_vec3("light.ambient", light.ambient);
		texture_shader.set_vec3("light.diffuse", light.diffuse);
		texture_shader.set_vec3("light.specular", light.specular);
		texture_shader.set_vec3("light.position", light.position);
		texture_shader.set_vec3("camera_position", main_window.get_camera_position());

		texture_shader.set_float("time", glfwGetTime());

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

		lighting_shader.set_vec3("light_color", light.ambient);
		lighting_shader.set_vec3("object_color", glm::vec3{1.0f, 1.0f, 1.0f});

		main_window.reset_model();
		main_window.translate_model(light.position);
		main_window.scale_model(0.05f);
		lighting_shader.set_mat4("model", main_window.model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//==== ground
		glBindTexture(GL_TEXTURE_2D, 0);

		ground_shader.use();
		ground_shader.set_mat4("view", main_window.view);
		ground_shader.set_mat4("projection", main_window.projection);

		glBindVertexArray(ground_VAO);

		ground_shader.set_int("ground_texture", 0);
		ground_texture.use(GL_TEXTURE0);

		ground_shader.set_vec3("light_color", light.ambient);
		ground_shader.set_vec3("object_color", glm::vec3{0.75f, 0.75f, 0.75f});

		main_window.reset_model();
		main_window.translate_model(0.0f, -1.0f, 0.0f);
		main_window.scale_model(10.0f);
		ground_shader.set_mat4("model", main_window.model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//==== mesh
		mesh_shader.use();

		mesh_shader.set_float("time", glfwGetTime());

		mesh_shader.set_vec3("light.ambient", light.ambient);
		mesh_shader.set_vec3("light.diffuse", light.diffuse);
		mesh_shader.set_vec3("light.specular", light.specular);
		mesh_shader.set_vec3("light.position", light.position);
		mesh_shader.set_vec3("camera_position", main_window.get_camera_position());

		main_window.reset_model();
		main_window.translate_model(3.0f, 1.0f, 0.0f);
		cube_mesh.draw(main_window, mesh_shader);

		//==== model
		model_shader.use();

		main_window.reset_model();
		main_window.translate_model(-3.0f, 1.0f, 0.0f);
		main_window.scale_model(0.1f);
		nanosuit.draw(main_window, model_shader);

		// poll current events and swap the buffers
		glfwPollEvents();
		main_window.swap_buffers();
	}

	glfwTerminate();
}
