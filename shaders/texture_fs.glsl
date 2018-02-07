#version 330 core
uniform sampler2D container_texture_sampler;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;
uniform bool use_texture;

in vec2 texture_coord;
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

struct Material {
	vec3 ambient_lighting;
	vec3 diffuse_lighting;
	vec3 specular_lighting;
	float shininess;
};

uniform Material material;

void main()
{
	// ambient lighting
	vec3 ambient_lighting = material.ambient_lighting * light_color;

	// diffuse lighting
	vec3 norm_normal = normalize(normal);
	vec3 norm_light_direction = normalize(light_position - fragment_position);
	float diff = max(dot(norm_normal, norm_light_direction), 0.0f);
	vec3 diffuse_lighting = (diff * material.diffuse_lighting) * light_color;

	// specular lighting
	vec3 norm_view_direction = normalize(camera_position - fragment_position);
	vec3 norm_reflected_view_direction = reflect(-norm_light_direction, norm_normal);
	float spec = pow(max(dot(norm_view_direction, norm_reflected_view_direction), 0.0f), material.shininess);
	vec3 specular_lighting = (material.specular_lighting * spec) * light_color;

	if (use_texture) {
		vec4 container_texture = texture(container_texture_sampler, texture_coord);
		vec4 total_lighting = vec4(ambient_lighting + diffuse_lighting + specular_lighting, 1.0f);
		fragment_color = container_texture * total_lighting;
	} else {
		vec4 object_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		fragment_color = object_color * vec4(ambient_lighting + diffuse_lighting + specular_lighting, 1.0f);
	}
}
