#include <iostream>

#include "glmesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
	: vertices(vertices)
	, indices(indices)
{
	this->textures = std::move(textures);

	setup_mesh();
	init_physics();
}

Mesh::~Mesh()
{
	if (rigid_body)
		delete rigid_body;

	if (motion_state)
		delete motion_state;

	if (shape)
		delete shape;
}

Mesh::Mesh(Mesh &&o) noexcept
	: vertices(std::move(o.vertices))
	, indices(std::move(o.indices))
	, textures(std::move(o.textures))
	, VAO(o.VAO), VBO(o.VBO), EBO(o.EBO)
	, mass(o.mass), shape(o.shape), motion_state(o.motion_state), inertia(o.inertia)
	, rigid_body(o.rigid_body)
{
	o.VAO = 0;
	o.VBO = 0;
	o.EBO = 0;

	o.shape = nullptr;
	o.motion_state = nullptr;
	o.rigid_body = nullptr;
}

void Mesh::init_physics()
{
	mass = 1.0f;
	inertia = btVector3(0.0f, 0.0f, 0.0f);

	shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
							    btVector3(0.0f, 10.0f, 0.0f)));
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo
		rigid_body_info(mass, motion_state, shape, inertia);
	rigid_body = new btRigidBody(rigid_body_info);
}

btTransform Mesh::get_transform()
{
	return rigid_body->getWorldTransform();
}

void Mesh::draw(Window &window, Shader &shader)
{
	shader.set_mat4("projection", window.projection);
	shader.set_mat4("view", window.view);
	shader.set_mat4("model", window.model);

	GLuint ndiffuse = 1;
	GLuint nspecular = 1;

	shader.use();

	for (unsigned i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(ndiffuse++);
		else if (name == "texture_specular")
			number = std::to_string(nspecular++);

		shader.set_int(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setup_mesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texture_coords));

	glBindVertexArray(0);
}
