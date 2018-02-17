#include <iostream>

#include "gllight.h"
#include "glshader.h"

std::shared_ptr<PointLight> point_light = std::shared_ptr<PointLight>
	(new PointLight {
		glm::vec3{0.0f, 1.0f, 0.0f},

		glm::vec3{0.1f, 0.1f, 0.1f},
		glm::vec3{0.8f, 0.8f, 0.8f},
		glm::vec3{0.5f, 0.5f, 0.5f}
	});

std::shared_ptr<DirectionalLight> dir_light = std::shared_ptr<DirectionalLight>
	(new DirectionalLight {
		glm::vec3{0.0f, -1.0f, 0.0f},

		glm::vec3{0.05f, 0.05f, 0.05f},
		glm::vec3{0.3f, 0.3f, 0.3f},
		glm::vec3{0.5f, 0.5f, 0.5f}
	});

std::shared_ptr<SpotLight> spot_light = std::shared_ptr<SpotLight>
	(new SpotLight {
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		12.5f,

		glm::vec3{0.3f, 0.3f, 0.3f},
		glm::vec3{0.3f, 0.3f, 0.3f},
		glm::vec3{0.5f, 0.5f, 0.5f}
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
	, constant(1.0f), linear(0.09f), quadratic(0.032f)
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
	shader.set_float((light_identifier + ".constant").c_str(), constant);
	shader.set_float((light_identifier + ".linear").c_str(), linear);
	shader.set_float((light_identifier + ".quadratic").c_str(), quadratic);
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float phi,
		     glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: Light(ambient, diffuse, specular, "spotlight")
	, position(position), direction(direction), phi(phi)
{
	;
}

SpotLight::~SpotLight()
{
	;
}

void SpotLight::update(glm::vec3 position, glm::vec3 direction, float phi)
{
	this->position = position;
	this->direction = direction;
	this->phi = phi;
}

void SpotLight::apply(const Shader &shader, unsigned nlight)
{
	Light::apply(shader, nlight);

	std::string nlight_str = std::to_string(nlight);
	std::string light_identifier = light_type + nlight_str;
	shader.set_vec3((light_identifier + ".position").c_str(), position);
	shader.set_vec3((light_identifier + ".direction").c_str(), direction);
	shader.set_float((light_identifier + ".cutoff").c_str(), glm::cos(glm::radians(phi)));
}
