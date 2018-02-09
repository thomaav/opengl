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
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
};

uniform Material material;
uniform Light light;

void main()
{
	// ambient lighting
	vec3 ambient_lighting = vec3(texture(material.texture_diffuse1, texture_coord)) * light.ambient;

	// diffuse lighting
	vec3 norm_normal = normalize(normal);
	vec3 norm_light_direction = normalize(light.position - fragment_position);
	float diff = max(dot(norm_normal, norm_light_direction), 0.0f);
	vec3 diffuse_lighting = vec3(texture(material.texture_diffuse1, texture_coord)) * diff * light.diffuse;

	// specular lighting
	vec3 norm_view_direction = normalize(camera_position - fragment_position);
	vec3 norm_reflected_view_direction = reflect(-norm_light_direction, norm_normal);
	float spec = pow(max(dot(norm_view_direction, norm_reflected_view_direction), 0.0f), 32.0f);
	vec3 specular_lighting = vec3(texture(material.texture_specular1, texture_coord)) * spec * light.specular;

	fragment_color = vec4(ambient_lighting + diffuse_lighting + specular_lighting, 1.0f);
}
