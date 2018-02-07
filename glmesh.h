#pragma once

#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();
	void draw(Window &window, Shader &shader);
private:
	GLuint VAO, VBO, EBO = 0;

	void setup_mesh();
};
