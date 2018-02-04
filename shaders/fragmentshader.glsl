#version 330 core
uniform vec4 cpu_color;

out vec4 fragment_color;

void main()
{
	fragment_color = cpu_color;
}
