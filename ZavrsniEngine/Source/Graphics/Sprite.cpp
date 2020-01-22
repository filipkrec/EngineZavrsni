#include "Sprite.h"

namespace graphics {

	Sprite::Sprite(float x, float y, float width, float height, const unsigned int color)
		: _position(math::Vector3(x, y, 0)), _size(math::Vector2(width, height)), _color(color),_texture(nullptr)
	{
	}

	Sprite::Sprite(float x, float y, float width, float height, const Texture* texture)
		: _position(math::Vector3(x, y, 0)), _size(math::Vector2(width, height)), _color(0xffffffff), _texture(texture)
	{
	}

}