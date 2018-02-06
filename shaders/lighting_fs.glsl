#version 330 core
uniform vec3 light_color;
uniform vec3 object_color;

out vec4 fragment_color;

void main()
{
	fragment_color = vec4(object_color * light_color, 1.0f);
}
