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

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();

	Mesh(const Mesh &o) = delete;
	Mesh & operator=(const Mesh &o) = delete;
	Mesh(Mesh &&o) noexcept;

	void init_physics();
	btTransform get_transform();

	void draw(Window &window, Shader &shader);
	    btRigidBody *rigid_body;
private:
	GLuint VAO, VBO, EBO = 0;

	float mass;
	btCollisionShape *shape;
	btDefaultMotionState *motion_state;
	btVector3 inertia;

	void setup_mesh();
};
