#include "Sprite.h"

namespace graphics {

	Sprite::Sprite(float x, float y, float width, float height, const unsigned int color)
		: _Position(math::Vector3(x, y, 0)), _Size(math::Vector2(width, height)), _Color(color)
	{
	}
}