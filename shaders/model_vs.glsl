#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 texture_coords_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texture_coords;
out vec3 normal;
out vec3 fragment_position;

void main()
{
	gl_Position = projection * view * model * vec4(vertex, 1.0);
	texture_coords = texture_coords_in;
	normal = mat3(transpose(inverse(model))) * normal_in;
	fragment_position = vec3(model * vec4(vertex, 1.0f));
}
