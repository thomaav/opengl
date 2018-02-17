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

	float constant;
	float linear;
	float quadratic;
};

uniform Light pointlight0;
uniform Light dirlight1;

vec3 diffuse_lighting(vec3 light_direction, vec3 diffuse)
{
	vec3 normalized_normal = normalize(normal);
	vec3 normalized_light_dir = normalize(light_direction);
	float diff = max(dot(normalized_normal, normalized_light_dir), 0.0f);
	return diffuse * diff;
}

vec3 specular_lighting(vec3 light_direction, vec3 specular)
{
	vec3 normalized_normal = normalize(normal);
	vec3 normalized_light_dir = normalize(-light_direction);
	vec3 normalized_view_dir = normalize(camera_position - fragment_position);
	vec3 normalized_reflected_view_dir = reflect(normalized_light_dir, normalized_normal);

	float spec = pow(max(dot(normalized_view_dir, normalized_reflected_view_dir), 0.0f), 32.0f);
	return specular * spec;
}

void main()
{
	vec3 norm_normal = normalize(normal);

	// point light
	vec3 pointlight_direction = pointlight0.position - fragment_position;

	vec3 ambient = pointlight0.ambient * vec3(texture(texture_diffuse1, texture_coords));
	vec3 diffuse = diffuse_lighting(pointlight_direction, pointlight0.diffuse)
		* vec3(texture(texture_diffuse1, texture_coords));
	vec3 specular = specular_lighting(pointlight_direction, pointlight0.specular)
		* vec3(texture(texture_diffuse1, texture_coords));

	float distance = length(pointlight0.position - fragment_position);
	float attenuation = 1.0f / (pointlight0.constant +
				    pointlight0.linear * distance +
				    pointlight0.quadratic * pow(distance, 2.0f));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// directional light
	vec3 dirlight_dir = normalize(-dirlight1.direction);

	vec3 dir_ambient = dirlight1.ambient * vec3(texture(texture_diffuse1, texture_coords));
	vec3 dir_diffuse = diffuse_lighting(dirlight_dir, dirlight1.diffuse)
		* vec3(texture(texture_diffuse1, texture_coords));
	vec3 dir_specular = specular_lighting(-dirlight_dir, dirlight1.specular)
		* vec3(texture(texture_specular1, texture_coords));

	vec3 point_sum = ambient + diffuse + specular;
	vec3 dir_sum = dir_ambient + dir_diffuse + dir_specular;

	fragment_color = vec4(point_sum + dir_sum, 1.0f);
}
