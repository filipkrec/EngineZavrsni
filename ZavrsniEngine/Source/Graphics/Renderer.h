#pragma once
#include <cstddef>
#include <vector>
#include <GL/glew.h>

#include "Sprite.h"
#include "../Math/Math.h"

namespace graphics {

#define SHADER_VERTEX_INDEX					0

	struct VertexData {
		math::Vector3 Position;
	};

	class Renderer
	{
	private:
		GLuint _VBO;
		VertexData* _vertexBufferData;

		int _count;
	public:
		Renderer();
		~Renderer();

		void begin();
		void submit(const Sprite* renderable);
		void end();
		void flush();
	private:
		void init();
	};

}





