#pragma once

#include "GameObject.h"
#include "../../Window.h"
#include "../../Util/Timer.h"
#include "Inventory/Weapon.h"

namespace objects {
	class Actor : public GameObject
	{
	protected:
		enum State
		{
		STATE_MOVING,
		STATE_DEAD,
		STATE_STILL
		};

		unsigned int _health;
		float _movementSpeed;
		float _movementSpeedMax;
		std::vector<unsigned int> _textureIds;

		State _state;
		Weapon* _weapon;
	protected:
		Actor();
		Actor(GameObject& gameObject, unsigned int health, float movementSpeedMax, const State& state);
		void addTexture(const graphics::Texture& texture);
		void moveWeapon();

		void move() override;
		void pickup(Pickup& pickup);
		virtual void animate() {};
		virtual void processState() {};
		virtual void process(const engine::Window& window) = 0;

	public:
		inline const unsigned int& getHealth() const { return _health; }
		inline const float& getMovementSpeed() const { return _movementSpeed; }
		inline const float& getMovementSpeedMax() const { return _movementSpeedMax; }
		inline const State& getState() const { return _state; }
		Weapon* getWeapon();

		void setHealth(unsigned int value);
		void setMovementSpeed(float value);
		void setMovementSpeedMax(float value);
		void setState(const State& state);
	};
}