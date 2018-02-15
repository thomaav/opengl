#version 330 core
uniform vec3 camera_position;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in vec2 texture_coords;
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

struct Light {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light pointlight0;
uniform Light dirlight1;

void main()
{
	// point light
	vec3 ambient = pointlight0.ambient * vec3(texture(texture_diffuse1, texture_coords));

	vec3 norm_normal = normalize(normal);
	vec3 norm_light_direction = normalize(pointlight0.position - fragment_position);
	float diff = max(dot(norm_normal, norm_light_direction), 0.0f);
	vec3 diffuse = pointlight0.diffuse * diff * vec3(texture(texture_diffuse1, texture_coords));

	vec3 norm_view_direction = normalize(camera_position - fragment_position);
	vec3 norm_reflected_view_direction = reflect(-norm_light_direction, norm_normal);
	float spec = pow(max(dot(norm_view_direction, norm_reflected_view_direction), 0.0f), 32.0f);
	vec3 specular = pointlight0.specular * spec * vec3(texture(texture_specular1, texture_coords));

	// directional light
	vec3 dirlight_dir = normalize(-dirlight1.direction);

	vec3 dir_ambient = dirlight1.ambient * vec3(texture(texture_diffuse1, texture_coords));

	float dir_diff = max(dot(norm_normal, dirlight_dir), 0.0f);
	vec3 dir_diffuse =
		dirlight1.diffuse * dir_diff * vec3(texture(texture_diffuse1, texture_coords));

	vec3 dir_reflected_view_direction = reflect(-dirlight_dir, norm_normal);
	float dir_spec = pow(max(dot(norm_view_direction, dir_reflected_view_direction), 0.0f), 32.0f);
	vec3 dir_specular =
		dirlight1.specular * dir_spec * vec3(texture(texture_specular1, texture_coords));

	vec3 point_sum = ambient + diffuse + specular;
	vec3 dir_sum = dir_ambient + dir_diffuse + dir_specular;

	fragment_color = vec4(point_sum + dir_sum, 1.0f);
}
