#pragma once
#include <cstring>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gllight.h"

bool load_shader_src(const std::string &filename, std::string &buf);
void shader_compilation_status(GLuint &shader);
void program_link_status(GLuint &program);

class Shader {
public:
	Shader(const GLchar *vertex_shader, const GLchar *fragment_shader);
	~Shader();

	Shader(const Shader &o) = delete;
	Shader & operator=(const Shader &o) = delete;

	void use() const;
	GLuint get_program() const;

	void set_int(const char *identifier, const int val) const;
	void set_float(const char *identifier, const float val) const;
	void set_bool(const char *identifier, const bool val) const;
	void set_4f(const char *identifier, float x, float y, float z, float w) const;
	void set_vec3(const char *identifier, glm::vec3 vec) const;
	void set_mat4(const char *identifier, const glm::mat4 &mat) const;

	void add_light(std::shared_ptr<Light> light);
	std::vector<std::shared_ptr<Light>> lights;
private:
	GLuint program;
};
