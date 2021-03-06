#version 330 core
uniform vec3 camera_position;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform samplerCube skybox;

in vec2 texture_coords;
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

struct Light {
	vec3 position;
	vec3 direction;
	float inner_cutoff;
	float outer_cutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Light pointlight0;
uniform Light dirlight1;
uniform Light spotlight2;

uniform bool reflective = false;

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

	if (reflective) {
		vec3 I = normalize(fragment_position - camera_position);
		vec3 R = reflect(I, normalize(normal));
		fragment_color = vec4(texture(skybox, R).rgb, 1.0f);
		return;
	}

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

	// spotlight
	vec3 spot_ambient = vec3(0.0f, 0.0f, 0.0f);
	vec3 spot_diffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 spot_specular = vec3(0.0f, 0.0f, 0.0f);

	vec3 spotlight_direction = normalize(spotlight2.position - fragment_position);
	spot_ambient = spotlight2.ambient * vec3(texture(texture_diffuse1, texture_coords));
	spot_diffuse = diffuse_lighting(spotlight_direction, spotlight2.diffuse)
		* vec3(texture(texture_diffuse1, texture_coords));
	spot_specular = specular_lighting(spotlight_direction, spotlight2.specular)
		* vec3(texture(texture_diffuse1, texture_coords));

	float theta = dot(spotlight_direction, normalize(-spotlight2.direction));
	float epsilon = spotlight2.inner_cutoff - spotlight2.outer_cutoff;
	float intensity = clamp((theta - spotlight2.outer_cutoff) / epsilon, 0.0f, 1.0f);

	spot_ambient *= intensity;
	spot_diffuse *= intensity;
	spot_specular *= intensity;

	vec3 point_sum = ambient + diffuse + specular;
	vec3 dir_sum = dir_ambient + dir_diffuse + dir_specular;
	vec3 spot_sum = spot_ambient + spot_diffuse + spot_specular;

	fragment_color = vec4(point_sum + dir_sum + spot_sum, 1.0f);
}
