#version 330 core
uniform vec3 camera_position;

in vec2 texture_coord;
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D diffuse_lighting;
	sampler2D specular_lighting;
	sampler2D emission_lighting;
	float shininess;
};

uniform Material material;
uniform Light light;
uniform float time;

void main()
{
	vec3 emission_lighting = vec3(0.0f, 0.0f, 0.0f);

	// ambient lighting
	vec3 ambient_lighting = vec3(texture(material.specular_lighting, texture_coord)) * light.ambient;

	// diffuse lighting
	vec3 norm_normal = normalize(normal);
	vec3 norm_light_direction = normalize(light.position - fragment_position);
	float diff = max(dot(norm_normal, norm_light_direction), 0.0f);
	vec3 diffuse_lighting = vec3(texture(material.specular_lighting, texture_coord)) * diff * light.diffuse;

	// specular lighting
	vec3 norm_view_direction = normalize(camera_position - fragment_position);
	vec3 norm_reflected_view_direction = reflect(-norm_light_direction, norm_normal);
	float spec = pow(max(dot(norm_view_direction, norm_reflected_view_direction), 0.0f), material.shininess);
	vec3 specular_lighting = vec3(texture(material.specular_lighting, texture_coord)) * spec * light.specular;

	if (vec3(texture(material.specular_lighting, texture_coord)).r == 0.0f) {
		float distance_to_camera = length(camera_position - fragment_position);
		float visibility = 1.0f * (1.0f - min(1.0f, (distance_to_camera / 4.0f)));
		vec3 emission_texture = vec3(texture(material.emission_lighting, texture_coord + vec2(0.0f, time)));
		emission_lighting = emission_texture * visibility;
	}

	fragment_color = vec4(ambient_lighting + diffuse_lighting + specular_lighting + emission_lighting, 1.0f);
}
