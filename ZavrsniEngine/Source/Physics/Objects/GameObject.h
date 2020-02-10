#pragma once

#include "../../Graphics/Sprite.h"
#include "../Hitbox.h"
#define FRICTION 1.0f
#define PROCESSING_INTERVAL 60

namespace physics{
	class GameObject {
		Hitbox const _hitbox;
	protected:
		unsigned int _weight;
		math::Vector3 _currentForce;
		math::Vector3 _previousForce;
	public:
		GameObject();
		~GameObject();
		GameObject(const GameObject& other);
		GameObject(const graphics::Sprite& sprite, unsigned int weight);
		GameObject(const graphics::Sprite& sprite, unsigned int weight, const Hitbox& hitbox);

		inline const math::Vector3& getPreviousForce() const { return _previousForce; }
		inline const Hitbox& getHitbox() const { return _hitbox; }
		void savePreviousForce();

		bool isHit(const Hitbox& other);
		bool isHit(const math::Vector2& point);
		void calculateColission(const math::Vector3& force);
		void collide(GameObject& other);
		void move();

		graphics::Sprite _sprite;
	};
}