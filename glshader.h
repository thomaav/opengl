#pragma once
#include <cstring>

#include <glad/glad.h>

bool load_shader_src(const std::string &filename, std::string &buf);
void shader_compilation_status(GLuint &shader);
void program_link_status(GLuint &program);

class Shader {
public:
	Shader(const GLchar *vertex_shader, const GLchar *fragment_shader);
	~Shader();
	void use();
	GLuint get_program();
private:
	GLuint program;
};
