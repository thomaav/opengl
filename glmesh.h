#pragma once

#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>

#include "glshader.h"
#include "gltexture.h"
#include "glwindow.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coords;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	glm::mat4 model;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();

	Mesh(const Mesh &o) = delete;
	Mesh & operator=(const Mesh &o) = delete;
	Mesh(Mesh &&o) noexcept;

	void reset_model();

	void translate_model(float x, float y, float z);
	void translate_model(glm::vec3 translation);

	void rotate_model(float radians, float x, float y, float z);
	void rotate_model(float radians, glm::vec3 rotation);

	void scale_model(float x, float y, float z);
	void scale_model(float scalar);

	void move_model(float x, float y, float z);

	void draw(Window &window, Shader &shader);
private:
	GLuint VAO, VBO, EBO = 0;

	void setup_mesh();
};
