#pragma once

#include <glad/glad.h>

class Texture {
public:
	Texture(const char *texture_fp);
	~Texture();
	void use();
	GLuint get_texture();
private:
	int width, height, nchannels;
	GLuint texture;
};
