#version 330 core
uniform vec4 cpu_color;
uniform sampler2D itexture;

in vec3 vertex_color;
in vec2 texture_coord;

out vec4 fragment_color;

void main()
{
	fragment_color = texture(itexture, texture_coord);
}
