#include <iostream>

#include "glskybox.h"
#include "include/stb_image.h"

constexpr float Skybox::vertices[];

Skybox::Skybox(std::vector<std::string> faces)
	: faces(faces)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (unsigned i = 0; i < faces.size(); ++i) {
		unsigned char *texture_data = stbi_load(faces[i].c_str(), &width, &height, &nchannels, 0);

		if (texture_data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
				     height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
			stbi_image_free(texture_data);
		} else {
			std::cout << "Failed to load cubemap texture " << faces[i] << std::endl;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

	glBindVertexArray(0);
}

Skybox::~Skybox()
{
	// TODO
	;
}

void Skybox::draw(Shader &skybox_shader, glm::mat4 view, glm::mat4 projection)
{
	glDepthFunc(GL_LEQUAL);

	skybox_shader.use();
	skybox_shader.set_int("skybox", 0);
	skybox_shader.set_mat4("view", glm::mat4(glm::mat3(view)));
	skybox_shader.set_mat4("projection", projection);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}
