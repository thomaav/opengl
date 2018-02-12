#version 330 core
uniform vec3 light_color;
uniform vec3 object_color;
uniform sampler2D ground_texture;

in vec2 texture_coord;

out vec4 fragment_color;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light0;

void main()
{
	vec3 ambient = light0.ambient * vec3(texture(ground_texture, texture_coord));
	fragment_color = vec4(ambient, 1.0f);
}
