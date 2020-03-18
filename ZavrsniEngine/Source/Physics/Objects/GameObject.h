#pragma once

#include "../../Graphics/Sprite.h"
#include "../Hitbox.h"
#include "Inventory/Weapon.h"

#define WEIGHTMAX 999999999

namespace objects {
	class Weapon;

	enum class ActorState
	{
		STATE_MOVING,
		STATE_DEAD,
		STATE_STILL
	};

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
		GameObject(const graphics::Sprite& sprite);
		GameObject(const graphics::Sprite& sprite, unsigned int weight, bool enviroment = false);
		GameObject(const graphics::Sprite& sprite, unsigned int weight, Shape shape, float width, float height = 0);

		inline const math::Vector3& getPreviousForce() const { return _previousForce; }
		void savePreviousForce();
		inline const math::Vector3& getCurrentForce() const { return _currentForce; }

		void setAllegiance(Allegiance allegiance);
		inline const Allegiance getAllegiance() const { return _allegiance; }


		inline const bool isCollsionOn() const { return _colissionOn; }
		void toggleColission();

		inline const unsigned int getWeight() const { return _weight; }

		virtual void onHit(const Weapon* weapon) {};

		void calculateColission(const math::Vector3& force);
		virtual void collide(GameObject& other);
		void calculateNextMove();
		virtual void move();

		virtual const ActorState& getState() const  { return ActorState::STATE_STILL; }

	};
}