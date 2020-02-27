#pragma once

#include "../../Graphics/Sprite.h"
#include "../Hitbox.h"

namespace objects {
	class GameObject : public Hitbox {
	protected:
		unsigned int _weight;
		math::Vector3 _currentForce;

		math::Vector3 _previousForce; //za racunanje kolizije
		float _currentSpeed; 

		bool _colissionOn;
		bool _colided;

	public:
		math::Vector2 _nextMove; //za racunanje kolizije

		GameObject();
		virtual ~GameObject();
		GameObject(graphics::Sprite* sprite);
		GameObject(graphics::Sprite* sprite, unsigned int weight);
		GameObject(graphics::Sprite* sprite, unsigned int weight, Shape shape, float width, float height = 0);

		void swapSprite(graphics::Sprite* sprite);

		inline const math::Vector3& getPreviousForce() const { return _previousForce; }
		void savePreviousForce();

		void toggleColission();
		void toggleColided();

		inline const bool  isColided() const { return _colided; }

		void calculateColission(const math::Vector3& force);
		void collide(GameObject& other);
		void calculateNextMove();
		virtual void move();

	};
}