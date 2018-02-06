#version 330 core
uniform sampler2D container_texture_sampler;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

in vec2 texture_coord;
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

void main()
{
	// ambient lighting
	float ambient_strength = 0.12f;
	vec3 ambient_lighting = ambient_strength * light_color;

	// diffuse lighting
	vec3 norm_normal = normalize(normal);
	vec3 norm_light_direction = normalize(light_position - fragment_position);
	float diff = max(dot(norm_normal, norm_light_direction), 0.0f);
	vec3 diffuse_lighting = diff * light_color;

	// specular lighting
	float specular_strength = 0.9f;
	vec3 norm_view_direction = normalize(camera_position - fragment_position);
	vec3 norm_reflected_view_direction = reflect(-norm_light_direction, norm_normal);
	float spec = pow(max(dot(norm_view_direction, norm_reflected_view_direction), 0.0f), 32);
	vec3 specular_lighting = specular_strength * spec * light_color;

	vec4 container_texture = texture(container_texture_sampler, texture_coord);
	fragment_color = container_texture * vec4(ambient_lighting + diffuse_lighting + specular_lighting, 1.0f);
}
