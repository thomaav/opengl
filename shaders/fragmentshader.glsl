#version 330 core
uniform sampler2D minecraft_texture_sampler;

in vec2 texture_coord;

out vec4 fragment_color;

void main()
{
	vec4 minecraft_texture = texture(minecraft_texture_sampler, texture_coord);
	fragment_color = minecraft_texture;
}
