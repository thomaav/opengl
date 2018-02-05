#pragma once
#include <cstring>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool load_shader_src(const std::string &filename, std::string &buf);
void shader_compilation_status(GLuint &shader);
void program_link_status(GLuint &program);

class Shader {
public:
	Shader(const GLchar *vertex_shader, const GLchar *fragment_shader);
	~Shader();
	void use() const;
	GLuint get_program() const;

	void set_int(const char *identifier, const int val) const;
	void set_mat4(const char *identifier, const glm::mat4 &mat) const;
private:
	GLuint program;
};
