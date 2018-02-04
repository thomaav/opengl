#version 330 core
uniform vec4 cpu_color;
in vec3 vertex_color;

out vec4 fragment_color;

void main()
{
	fragment_color = vec4(vertex_color, 1.0);
}
