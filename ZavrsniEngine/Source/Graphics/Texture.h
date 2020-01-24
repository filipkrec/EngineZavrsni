#pragma once	
#include <string>
#include <FreeImage.h>
#include <GL/glew.h>
#include <vector>

#include "../Math/Math.h"


namespace graphics {
	class Texture {
		GLuint _textureId;
	public:
		Texture(const std::string& filename, const bool& repeat = false);
		~Texture();

		inline const GLuint getId() const { return _textureId; }
	private:
		GLuint load(const char* filename, const bool& repeat);
	};
}