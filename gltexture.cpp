#include <iostream>

#include "include/stb_image.h"
#include "gltexture.h"

Texture::Texture(const std::string texture_fp)
	: texture_fp(texture_fp)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	unsigned char *texture_data = stbi_load(texture_fp.c_str(), &width, &height, &nchannels, 0);

	if (texture_data) {
		switch (nchannels) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			std::cout << "Invalid number of channels in texture: " << texture_fp << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		std::cout << "Failed to load texture " << texture_fp << std::endl;
	}

	stbi_image_free(texture_data);
	glBindTexture(GL_TEXTURE_2D, 0);
}


Texture::~Texture()
{
	if (id)
		glDeleteTextures(1, &id);
}

Texture::Texture(Texture &&o) noexcept
	: id(std::move(o.id))
	, type(std::move(o.type))
	, texture_fp(std::move(o.texture_fp))
	, width(std::move(o.width)), height(std::move(o.height)), nchannels(std::move(o.nchannels))
	, format(std::move(o.format))
{
	o.id = 0;
}

void Texture::use(GLenum unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, id);
}
