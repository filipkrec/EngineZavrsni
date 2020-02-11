#pragma once

#include "../../Graphics/Sprite.h"
#include "../Hitbox.h"
#define FRICTION 1.0f
#define PROCESSING_INTERVAL 60

namespace physics{
	class GameObject : public Hitbox {
	protected:
		unsigned int _weight;
		math::Vector3 _currentForce;

		math::Vector3 _previousForce; //za racunanje kolizije
		float _currentSpeed; 
	public:
		math::Vector2 _nextMove; //za racunanje kolizije

		GameObject();
		~GameObject();
		GameObject(graphics::Sprite* sprite, unsigned int weight);
		GameObject(graphics::Sprite* sprite, unsigned int weight, Shape shape, float width, float height = 0);

		inline const math::Vector3& getPreviousForce() const { return _previousForce; }
		void savePreviousForce();

		void calculateColission(const math::Vector3& force);
		void collide(GameObject& other);
		void calculateNextMove();
		void move();

	};
}