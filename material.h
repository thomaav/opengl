#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Material {
	glm::vec3 ambient_lighting;
	glm::vec3 diffuse_lighting;
	glm::vec3 specular_lighting;
	float shininess;
};

extern Material texture;
extern Material emerald;
