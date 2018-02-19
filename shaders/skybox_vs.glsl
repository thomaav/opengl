#version 330 core
layout (location = 0) in vec3 vertex;

uniform mat4 projection;
uniform mat4 view;

out vec3 texture_coord;

void main()
{
	gl_Position = projection * view * vec4(vertex, 1.0f);
	texture_coord = vertex;
}
