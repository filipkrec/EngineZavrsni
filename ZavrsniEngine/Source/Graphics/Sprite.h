#pragma once
#include <vector>
#include "../Math/Math.h"
#include "Texture.h"

namespace graphics {
	class Sprite
	{
	protected:
		math::Vector2 _position[4];
		math::Vector2 _size;
		unsigned int _color;
		unsigned int  _zindex;
		const Texture* _texture;

		math::Vector2 _textureCoordinates[4];
		friend class Group;
	private:
	public:
		Sprite(float x, float y, float width, float height,  const unsigned int color, float zindex = 0.0f);
		Sprite(float x, float y, float width, float height, const Texture* texture, float zindex = 0.0f);

		void Rotate(float degrees);
		void RotatePosition(float degrees);

		void setPosition(const math::Vector2& value,const int& index);
		void setTextureCoordinates(const math::Vector2& value, const int& index);
		void setTextureId(const int& index);
		void setZindex(const unsigned int& zindex);

		inline const math::Vector2* getPosition() const { return _position; }
		inline const math::Vector2& getSize() const { return _size; }
		inline const unsigned int& getColor() const { return _color; }
		inline const unsigned int& getZindex() const { return _zindex; }
		inline const Texture* getTexture() const { return _texture; }
		inline const math::Vector2* getTextureCoordinates() const { return _textureCoordinates; }
	};
}