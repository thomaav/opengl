#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texture_coord_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texture_coord;
out vec3 fragment_position;

void main()
{
	gl_Position = projection * view * model * vec4(vertex, 1.0f);
	texture_coord = texture_coord_in;
	fragment_position = vec3(model * vec4(vertex, 1.0f));
}
