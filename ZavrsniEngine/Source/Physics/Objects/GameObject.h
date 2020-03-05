#pragma once

#include "../../Graphics/Sprite.h"
#include "../Hitbox.h"

namespace objects {
	enum class Allegiance {
		GOOD,
		BAD,
		NEUTRAL,
		ENVIROMENT
	};

	class GameObject : public Hitbox {
	protected:
		unsigned int _weight;
		math::Vector3 _currentForce;

		math::Vector3 _previousForce; //za racunanje kolizije
		float _currentSpeed; 

		bool _colissionOn;
		Allegiance _allegiance;

	public:
		math::Vector2 _nextMove; //za racunanje kolizije

		GameObject();
		virtual ~GameObject();
		GameObject(graphics::Sprite* sprite);
		GameObject(graphics::Sprite* sprite, unsigned int weight);
		GameObject(graphics::Sprite* sprite, unsigned int weight, Shape shape, float width, float height = 0);

		inline const math::Vector3& getPreviousForce() const { return _previousForce; }
		void savePreviousForce();

		void setAllegiance(Allegiance allegiance);
		inline const Allegiance getAllegiance() const { return _allegiance; }
		void toggleColission();


		inline const unsigned int getWeight() const { return _weight; }


		void calculateColission(const math::Vector3& force);
		virtual void collide(GameObject& other);
		void calculateNextMove();
		virtual void move();


	};
}