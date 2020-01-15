#pragma once
#include <iostream>
#include "../../Window.h"
#include "../../Math/Math.h"

namespace graphics {
	class Shader
	{
	private:
		GLuint _shaderID;
	public:
		Shader(const char* vertPath, const char* fragPath);
		~Shader();

		void setUniform1f(const GLchar* name, float value);
		void setUniform1fv(const GLchar* name, int count, float* value);
		void setUniform1iv(const GLchar* name, int count, int* value);
		void setUniform1i(const GLchar* name, int value);
		void setUniform2f(const GLchar* name, const math:: Vector2& vector);
		void setUniform3f(const GLchar* name, const math::Vector3& vector);
		void setUniform4f(const GLchar* name, const math::Vector4& vector);
		void setUniformMat4(const GLchar* name, const math::Matrix4& matrix);

		void enable() const;
		void disable() const;
	private:
		unsigned int load(const char* vertPath, const char* fragPath);
		GLint GetUniformLocation(const GLchar* name);
	};
}