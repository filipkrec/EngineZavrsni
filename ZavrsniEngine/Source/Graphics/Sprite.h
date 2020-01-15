#pragma once
#include "../Math/Math.h"

namespace graphics {
	class Sprite
	{
	protected:
		math::Vector3 _Position;
		math::Vector2 _Size;
		unsigned int _Color;
	private:
	public:
		Sprite(float x, float y, float width, float height, const unsigned int color);

		inline const math::Vector3& getPosition() const { return _Position; }
		inline const math::Vector2& getSize() const { return _Size; }
		inline const unsigned int& getColor() const { return _Color; }
	};
}