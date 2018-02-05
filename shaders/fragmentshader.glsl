#version 330 core
uniform sampler2D container_texture_sampler;
uniform sampler2D awesomeface_texture_sampler;

in vec3 vertex_color;
in vec2 texture_coord;

out vec4 fragment_color;

void main()
{
	vec4 container_texture = texture(container_texture_sampler, texture_coord);
	vec4 awesomeface_texture = texture(awesomeface_texture_sampler, texture_coord);

	fragment_color = mix(container_texture, awesomeface_texture, awesomeface_texture.a * 0.2f);
}
