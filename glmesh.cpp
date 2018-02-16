#include <iostream>

#include "glmesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
	: vertices(vertices)
	, indices(indices)
{
	this->textures = std::move(textures);

	setup_mesh();
}

Mesh::~Mesh()
{
	;
}

Mesh::Mesh(Mesh &&o) noexcept
	: vertices(std::move(o.vertices))
	, indices(std::move(o.indices))
	, textures(std::move(o.textures))
	, VAO(o.VAO), VBO(o.VBO), EBO(o.EBO)
{
	o.VAO = 0;
	o.VBO = 0;
	o.EBO = 0;
}

void Mesh::reset_model()
{
	model = glm::mat4{};
}

void Mesh::translate_model(float x, float y, float z)
{
	model = glm::translate(model, glm::vec3{x, y, z});
}

void Mesh::translate_model(glm::vec3 translation)
{
	model = glm::translate(model, translation);
}

void Mesh::rotate_model(float radians, float x, float y, float z)
{
	model = glm::rotate(model, radians, glm::vec3{x, y, z});
}

void Mesh::rotate_model(float radians, glm::vec3 rotation)
{
	model = glm::rotate(model, radians, rotation);
}

void Mesh::scale_model(float x, float y, float z)
{
	model = glm::scale(model, glm::vec3{x, y, z});
}

void Mesh::scale_model(float scalar)
{
	model = glm::scale(model, glm::vec3(scalar, scalar, scalar));
}

void Mesh::draw(Window &window, Shader &shader)
{
	shader.set_mat4("projection", window.projection);
	shader.set_mat4("view", window.view);
	shader.set_mat4("model", model);

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
