#version 330 core
uniform sampler2D container_texture_sampler;
uniform vec3 light_color;
uniform vec3 light_position;

in vec2 texture_coord;
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

void main()
{
	// ambient lighting
	float ambient_strength = 0.2f;
	vec3 ambient_lighting = ambient_strength * light_color;

	// diffuse lighting
	vec3 norm_normal = normalize(normal);
	vec3 norm_light_direction = normalize(light_position - fragment_position);
	float diff = max(dot(norm_normal, norm_light_direction), 0.0);
	vec3 diffuse_lighting = diff * light_color;

	vec4 container_texture = texture(container_texture_sampler, texture_coord);
	fragment_color = container_texture * vec4(ambient_lighting + diffuse_lighting, 1.0f);
}
