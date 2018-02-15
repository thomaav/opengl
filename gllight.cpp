#include <iostream>

#include "gllight.h"
#include "glshader.h"

std::shared_ptr<PointLight> point_light = std::shared_ptr<PointLight>
	(new PointLight {
		glm::vec3{0.0f, 1.0f, 0.0f},

		glm::vec3{0.1f, 0.1f, 0.1f},
		glm::vec3{0.8f, 0.8f, 0.8f},
		glm::vec3{1.0f, 1.0f, 1.0f}
	});

std::shared_ptr<DirectionalLight> dir_light = std::shared_ptr<DirectionalLight>
	(new DirectionalLight {
		glm::vec3{0.0f, -1.0f, 0.0f},

		glm::vec3{0.05f, 0.05f, 0.05f},
		glm::vec3{0.3f, 0.3f, 0.3f},
		glm::vec3{1.0f, 1.0f, 1.0f}
	});

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string light_type)
	: ambient(ambient)
	, diffuse(diffuse)
	, specular(specular)
	, light_type(light_type)
{
	;
}

Light::~Light()
{
	;
}

void Light::apply(const Shader &shader, unsigned nlight)
{
	std::string nlight_str = std::to_string(nlight);
	std::string light_identifier = light_type + nlight_str;

	shader.set_vec3((light_identifier + ".ambient").c_str(), ambient);
	shader.set_vec3((light_identifier + ".diffuse").c_str(), diffuse);
	shader.set_vec3((light_identifier + ".specular").c_str(), specular);
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient,
				   glm::vec3 diffuse, glm::vec3 specular)
	: Light(ambient, diffuse, specular, "dirlight")
	, direction(direction)
{
	;
}

DirectionalLight::~DirectionalLight()
{
	;
}

void DirectionalLight::apply(const Shader &shader, unsigned nlight)
{
	Light::apply(shader, nlight);

	std::string nlight_str = std::to_string(nlight);
	std::string light_identifier = light_type + nlight_str;
	shader.set_vec3((light_identifier + ".direction").c_str(), direction);
}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient,
		       glm::vec3 diffuse, glm::vec3 specular)
	: Light(ambient, diffuse, specular, "pointlight")
	, position(position)
{
	;
}

PointLight::~PointLight()
{
	;
}

void PointLight::apply(const Shader &shader, unsigned nlight)
{
	Light::apply(shader, nlight);

	std::string nlight_str = std::to_string(nlight);
	std::string light_identifier = light_type + nlight_str;
	shader.set_vec3((light_identifier + ".position").c_str(), position);
}
