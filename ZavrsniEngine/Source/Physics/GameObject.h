#pragma once

#include "../Graphics/Sprite.h"
#include "Hitbox.h"
#define FRICTION 0.8f

namespace physics{
	class GameObject {
		Hitbox const* _hitbox;
		unsigned int _weight;
		math::Vector3 _currentForce;

	public:
		GameObject(graphics::Sprite* sprite, unsigned int weight = 0, Hitbox* hitbox = nullptr);

		bool isHit(const Hitbox& other);
		bool isHit(const math::Vector2& point);
		void calculateColission(const math::Vector3& force);
		void move();

		graphics::Sprite* _sprite;
	};
}