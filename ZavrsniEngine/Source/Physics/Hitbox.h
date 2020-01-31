#pragma once
#include "../Math/Math.h"
#include "../Graphics/Sprite.h"

namespace physics {
	enum Shape {
		SQUARE = 1,
		CIRCLE = 2,
		CUSTOM = 3
	};

	class Hitbox {
		Shape _shape;
		math::Vector2 _collisionRange;
		const graphics::Sprite* _boundSprite;
	public:
		~Hitbox();
		Hitbox(graphics::Sprite* sprite, Shape shape, const float& width, const float& height = 0);
		bool isHit(const Hitbox& other);
		bool isHit(const math::Vector2& point);
	};
}