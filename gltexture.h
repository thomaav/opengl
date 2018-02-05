#pragma once

#include <glad/glad.h>

class Texture {
public:
	Texture(const char *texture_fp, bool alpha);
	~Texture();
	void use(GLenum unit);
	GLuint get_texture();
private:
	int width, height, nchannels;
	GLenum format;
	GLuint texture;
};
