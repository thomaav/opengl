#version 330 core
uniform sampler2D container_texture_sampler;
uniform vec3 light_color;

in vec2 texture_coord;

out vec4 fragment_color;

void main()
{
	vec4 container_texture = texture(container_texture_sampler, texture_coord);
	fragment_color = container_texture * vec4(light_color, 1.0f);
}
