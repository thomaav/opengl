#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_coord_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 transform;

out vec3 vertex_color;
out vec2 texture_coord;

void main()
{
	gl_Position = projection * view * model * transform * vec4(vertex, 1.0);
	vertex_color = color;
	texture_coord = texture_coord_in;
}
