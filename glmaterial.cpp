#include "glmaterial.h"

Material emerald = {
	.ambient_lighting = glm::vec3{0.215f, 0.1745f, 0.0215f},
	.diffuse_lighting = glm::vec3{0.07568f, 0.61424f, 0.07568f},
	.specular_lighting = glm::vec3{0.633f, 0.727811f, 0.633f},
	.shininess = 32.0f,
};

Material steel_container = {
	.shininess = 64.0f,
};
