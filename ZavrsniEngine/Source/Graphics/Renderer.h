#pragma once
#include <cstddef>
#include <vector>
#include <GL/glew.h>

#include "Sprite.h"
#include "../Math/Math.h"
#include "Shaders/Shader.h"

namespace graphics {

#define SHADER_VERTEX_INDEX					0
#define SHADER_TEXTURE_SLOT_INDEX			2
#define SHADER_TEXTURE_CORDS_INDEX			1
#define SHADER_COLOR_INDEX					3

#define MAX_SPRITES							10000
#define MAX_INDEX_BUFFER_SIZE				sizeof(unsigned int) * MAX_SPRITES
#define MAX_VERTEX_BUFFER_SIZE				sizeof(VertexData) * MAX_SPRITES

	struct VertexData {
		math::Vector3 Position;
		unsigned int Color;
		float TextureSlot;
		math::Vector2 TextureCoordinate;

		bool operator==(const VertexData& right)
		{
			return (Position == right.Position && Color == right.Color && TextureSlot == right.TextureSlot);
		}
	};

	class Renderer
	{
	private:
		GLuint _VBO;
		GLuint _IBO;
		GLuint _VAO;

		unsigned int* _indices;
		VertexData* _vertexBufferData;
		VertexData* _vertexBufferDataBegin;

		unsigned int _count;
		unsigned int _indexCount;
		std::vector<GLuint> _textures;
		std::vector<math::Vector2> _defaultCoordinates;
	private: 
		void init();
		unsigned int getTextureSlot(unsigned int textureId);
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





