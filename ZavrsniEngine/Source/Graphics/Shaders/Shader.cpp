#include <vector>
#include "../../Util/FileUtils.h"
#include "Shader.h"
using namespace math;

namespace graphics {
	Shader::Shader(const char* vertPath, const char* fragPath)
	{
		_shaderID = load(vertPath, fragPath);
	}

	Shader::~Shader() {
		glDeleteProgram(_shaderID);
	}
	GLuint Shader::load(const char* vertPath, const char* fragPath)
	{
		unsigned int program = glCreateProgram();
		unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
		const char* vertexSource = read_file(vertPath);
		const char* fragmentSource = read_file(fragPath);

		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);

		GLint result;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(vertex,GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(vertex,length,&length, &error[0]);
			std::cout << "Failed to compile vertex shader" << &error[0] << std::endl;
			glDeleteShader(vertex);
			return 0;
		}

		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(fragment,GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(fragment,length,&length, &error[0]);
			std::cout << "Failed to compile fragment shader" << &error[0] << std::endl;
			glDeleteShader(fragment);
			return 0;
		}

		glAttachShader(program,vertex);
		glAttachShader(program,fragment);
		glLinkProgram(program);
		glValidateProgram(program);
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
	}

	GLint Shader::GetUniformLocation(const GLchar* name) {
		return glGetUniformLocation(_shaderID, name);
	}

	void Shader::setUniform1f(const GLchar* name, float value) {
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::setUniform1fv(const GLchar* name, int count, float* value) {
		glUniform1fv(GetUniformLocation(name), count, value);
	}

	void Shader::setUniform1iv(const GLchar* name, int count, int* value) {
		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void Shader::setUniform1i(const GLchar* name, int value) {
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::setUniform2f(const GLchar* name, const Vector2& vector) {
		glUniform2f(GetUniformLocation(name), vector.x, vector.y);
	}

	void Shader::setUniform3f(const GLchar* name, const Vector3& vector) {
		glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void Shader::setUniform4f(const GLchar* name, const Vector4& vector) {
		glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::setUniformMat4(const GLchar* name, const Matrix4& matrix) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.elements);
	}

	void Shader::enable() const
	{
		glUseProgram(_shaderID);
	}

	void Shader::disable() const
	{
		glUseProgram(0);
	}
}