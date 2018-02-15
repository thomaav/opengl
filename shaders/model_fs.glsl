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

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light pointlight0;

void main()
{
	vec3 ambient = pointlight0.ambient * vec3(texture(texture_diffuse1, texture_coords));

	vec3 norm_normal = normalize(normal);
	vec3 norm_light_direction = normalize(pointlight0.position - fragment_position);
	float diff = max(dot(norm_normal, norm_light_direction), 0.0f);
	vec3 diffuse = pointlight0.diffuse * diff * vec3(texture(texture_specular1, texture_coords));

	vec3 norm_view_direction = normalize(camera_position - fragment_position);
	vec3 norm_reflected_view_direction = reflect(-norm_light_direction, norm_normal);
	float spec = pow(max(dot(norm_view_direction, norm_reflected_view_direction), 0.0f), 32.0f);
	vec3 specular = pointlight0.specular * spec * vec3(texture(texture_specular1, texture_coords));

	fragment_color = vec4(ambient + diffuse + specular, 1.0f);
}
