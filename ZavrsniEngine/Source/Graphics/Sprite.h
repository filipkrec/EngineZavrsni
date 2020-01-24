#pragma once
#include <vector>
#include "../Math/Math.h"
#include "Texture.h"

namespace graphics {
	class Sprite
	{
	protected:
		math::Vector3 _position[4];
		math::Vector2 _size;
		unsigned int _color;
		const Texture* _texture;

		math::Vector2 _textureCoordinates[4];
	private:
	public:
		Sprite(float x, float y, float width, float height, float zindex, const unsigned int color);
		Sprite(float x, float y, float width, float height, float zindex, const Texture* texture);

		void Rotate(float degrees);
		void RotatePosition(float degrees);

		inline const math::Vector3* getPosition() const { return _position; }
		inline const math::Vector2& getSize() const { return _size; }
		inline const unsigned int& getColor() const { return _color; }
		inline const Texture* getTexture() const { return _texture; }
		inline const math::Vector2* getTextureCoordinates() const { return _textureCoordinates; }
	};
}