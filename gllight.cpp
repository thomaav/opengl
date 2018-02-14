#include <iostream>

#include "gllight.h"
#include "glshader.h"

std::shared_ptr<PointLight> light = std::shared_ptr<PointLight>
	(new PointLight {
		glm::vec3{0.0f, 1.0f, 0.0f},

		glm::vec3{0.5f, 0.5f, 0.5f},
		glm::vec3{0.5f, 0.5f, 0.5f},
		glm::vec3{1.0f, 1.0f, 1.0f}
	});

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: ambient(ambient)
	, diffuse(diffuse)
	, specular(specular)
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
	std::string light_identifier = "light" + nlight_str;

	shader.set_vec3((light_identifier + ".ambient").c_str(), ambient);
	shader.set_vec3((light_identifier + ".diffuse").c_str(), diffuse);
	shader.set_vec3((light_identifier + ".specular").c_str(), specular);
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient,
				   glm::vec3 diffuse, glm::vec3 specular)
	: Light(ambient, diffuse, specular)
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
	std::string light_identifier = "light" + nlight_str;
	shader.set_vec3((light_identifier + ".direction").c_str(), direction);
}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient,
		       glm::vec3 diffuse, glm::vec3 specular)
	: Light(ambient, diffuse, specular)
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
	std::string light_identifier = "light" + nlight_str;
	shader.set_vec3((light_identifier + ".position").c_str(), position);
}
