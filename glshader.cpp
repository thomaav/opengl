#include <fstream>
#include <sstream>
#include <iostream>

#include "glshader.h"

bool load_shader_src(const std::string &filename, std::string &buf)
{
	std::ifstream f;
	f.open(filename.c_str());

	if (!f)
		return false;

	std::stringstream stream;
	stream << f.rdbuf();
	buf = stream.str();

	f.close();
	return true;
}

void shader_compilation_status(GLuint &shader)
{
	int success; char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	}
}

void program_link_status(GLuint &program)
{
	int success; char info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}
}

Shader::Shader(const GLchar *vertex_fn, const GLchar *fragment_fn)
{
	std::string vertex_shader_src, fragment_shader_src;

	load_shader_src(vertex_fn, vertex_shader_src);
	load_shader_src(fragment_fn, fragment_shader_src);

	const char *vsc_c_str = vertex_shader_src.c_str();
	const char *fsc_c_str = fragment_shader_src.c_str();

	GLuint vertex_shader, fragment_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vsc_c_str, NULL);
	glCompileShader(vertex_shader);
	shader_compilation_status(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fsc_c_str, NULL);
	glCompileShader(fragment_shader);
	shader_compilation_status(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	program_link_status(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
	if (program)
		glDeleteProgram(program);
}

void Shader::use() const
{
	glUseProgram(program);
}

GLuint Shader::get_program() const
{
	return program;
}

void Shader::set_int(const char *identifier, const int val) const
{
	GLuint location = glGetUniformLocation(program, identifier);
	glUniform1i(location, val);
}

void Shader::set_4f(const char *identifier, float x, float y, float z, float w) const
{
	GLuint location = glGetUniformLocation(program, identifier);
	glUniform4f(location, x, y, z, w);
}

void Shader::set_vec3(const char *identifier, glm::vec3 vec) const
{
	GLuint location = glGetUniformLocation(program, identifier);
	glUniform3fv(location, 1, &vec[0]);
}

void Shader::set_mat4(const char *identifier, const glm::mat4 &mat) const
{
	GLuint location = glGetUniformLocation(program, identifier);
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}
