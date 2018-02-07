#version 330 core
uniform vec3 light_color;
uniform vec3 object_color;
uniform sampler2D ground_texture;

in vec2 texture_coord;

out vec4 fragment_color;

void main()
{
	fragment_color = texture(ground_texture, texture_coord) * vec4(0.5f, 0.5f, 0.5f, 1.0f);
}
