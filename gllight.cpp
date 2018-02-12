#include "gllight.h"

std::shared_ptr<Light> light = std::shared_ptr<Light>
	(new Light {
		glm::vec3{0.0f, 1.0f, 0.0f},

		glm::vec3{0.2f, 0.2f, 0.2f},
		glm::vec3{0.5f, 0.5f, 0.5f},
		glm::vec3{1.0f, 1.0f, 1.0f}
	});

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: position(position)
	, ambient(ambient)
	, diffuse(diffuse)
	, specular(specular)
{
	;
}

Light::~Light()
{
	;
}
