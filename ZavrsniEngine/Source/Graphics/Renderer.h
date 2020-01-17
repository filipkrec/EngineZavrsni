#pragma once
#include <cstddef>
#include <vector>
#include <GL/glew.h>

#include "Sprite.h"
#include "../Math/Math.h"

namespace graphics {

#define SHADER_VERTEX_INDEX					0
#define MAX_SPRITES							100
#define MAX_INDEX_BUFFER_SIZE				sizeof(unsigned int) * MAX_SPRITES
#define MAX_VERTEX_BUFFER_SIZE				sizeof(VertexData) * MAX_SPRITES

	struct VertexData {
		math::Vector3 Position;

		bool operator==(const VertexData& right)
		{
			return (Position == right.Position);
		}
	};

	class Renderer
	{
	private:
		GLuint _VBO;
		GLuint _IBO;
		int test = 0;

		unsigned int* _indices;
		VertexData* _vertexBufferData;
		VertexData* _vertexBufferDataBegin;

		unsigned int _count;
		unsigned int _indexCount;
	private: 
		void init();
	public:
		Renderer();
		~Renderer();

		void begin();
		void submit(const Sprite* renderable);
		void end();
		void flush();
	private:
		const unsigned int& setIndex(VertexData& current);
	};

}





