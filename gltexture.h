#pragma once

#include <string>

#include <glad/glad.h>

class Texture {
public:
	GLuint id = 0;
	std::string type;
	std::string texture_fp;
	bool alpha;

	Texture(const std::string texture_fp, bool alpha);
	~Texture();

	Texture(const Texture &o) = delete;
	Texture & operator=(const Texture &o) = delete;
	Texture(Texture &&o) noexcept;

	void use(GLenum unit);
private:
	int width, height, nchannels;
	GLenum format;
};
