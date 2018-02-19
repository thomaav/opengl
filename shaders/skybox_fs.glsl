#version 330 core
in vec3 texture_coord;

uniform samplerCube skybox;

out vec4 fragment_color;

void main()
{
	fragment_color = texture(skybox, texture_coord);
}
