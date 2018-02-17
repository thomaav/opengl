#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// there is a circular dependency to apply lights to a
// shader. however, we only take a reference to said shader in
// Light::apply so that the compiler will know how much memory to
// allocate.
class Shader;

class Light {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	const std::string light_type;

	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string light_type);
	virtual ~Light();

	virtual void apply(const Shader &shader, unsigned nlight);
};

class DirectionalLight : public Light {
public:
	glm::vec3 direction;

	DirectionalLight(glm::vec3 position, glm::vec3 ambient,
			 glm::vec3 diffuse, glm::vec3 specular);
	~DirectionalLight();

	virtual void apply(const Shader &shader, unsigned nlight);
};

class PointLight : public Light {
public:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	PointLight(glm::vec3 position, glm::vec3 ambient,
		   glm::vec3 diffuse, glm::vec3 specular);
	~PointLight();

	virtual void apply(const Shader &shader, unsigned nlight);
};

class SpotLight : public Light {
public:
	glm::vec3 position;
	glm::vec3 direction;
	float phi;

	SpotLight(glm::vec3 position, glm::vec3 direction, float phi,
		  glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~SpotLight();

	void update(glm::vec3 position, glm::vec3 direction, float phi);

	virtual void apply(const Shader &shader, unsigned nlight);
};

extern std::shared_ptr<PointLight> point_light;
extern std::shared_ptr<DirectionalLight> dir_light;
extern std::shared_ptr<SpotLight> spot_light;
