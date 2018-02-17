#version 330 core
uniform vec3 camera_position;
uniform vec3 light_color;
uniform vec3 object_color;
uniform sampler2D ground_texture;

in vec2 texture_coord;
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
};

uniform Light pointlight0;
uniform Light spotlight1;

vec3 normal = vec3(0.0f, 1.0f, 0.0f);

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
	// point light
	vec3 ambient = pointlight0.ambient * vec3(texture(ground_texture, texture_coord));

	// spot light
	vec3 spot_ambient = vec3(0.0f, 0.0f, 0.0f);
	vec3 spot_diffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 spot_specular = vec3(0.0f, 0.0f, 0.0f);

	vec3 spotlight_direction = normalize(spotlight1.position - fragment_position);
	spot_ambient = spotlight1.ambient * vec3(texture(ground_texture, texture_coord));
	spot_diffuse = diffuse_lighting(spotlight_direction, spotlight1.diffuse)
		* vec3(texture(ground_texture, texture_coord));
	spot_specular = specular_lighting(spotlight_direction, spotlight1.specular)
		* vec3(texture(ground_texture, texture_coord));

	float theta = dot(spotlight_direction, normalize(-spotlight1.direction));
	float epsilon = spotlight1.inner_cutoff - spotlight1.outer_cutoff;
	float intensity = clamp((theta - spotlight1.outer_cutoff) / epsilon, 0.0f, 1.0f);

	spot_ambient *= intensity;
	spot_diffuse *= intensity;
	spot_specular *= intensity;

	vec3 point_sum = ambient;
	vec3 spot_sum = spot_ambient + spot_diffuse + spot_specular;

	fragment_color = vec4(point_sum + spot_sum, 1.0f);
}
