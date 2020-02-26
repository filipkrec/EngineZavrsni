#pragma once

#include "GameObject.h"
#include "../../Window.h"
#include "../../Util/Timer.h"
#include "Inventory/Pickup.h"
#include "Inventory/Weapon.h"
#include "../../Math/MathUtil.h"
#define MOVEMENT_SPEED_COEFFICIENT 0.05

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
		float _sightAngle;
		float _sightRange;
		std::vector<unsigned int> _textureIds;
		std::vector<Pickup*> _pickUpable;
		std::vector<GameObject*> _sighted;

		math::Vector2 _moveToPoint;
		math::Vector2 _moveToCheckPoint;

		void (*_onSight)(GameObject*);

		State _state;
		Weapon* _weapon;
	protected:
		Actor();
		Actor(GameObject& gameObject, unsigned int health, float _movementSpeed, const State& state);
		void addTexture(const graphics::Texture& texture);
		void moveWeapon();
		void rotateToPoint(math::Vector2);

		virtual void move() override;
		virtual void pickup(Pickup& pickup);
		virtual void animate() {};
		virtual void processState() {};
		void processSight();
		virtual void process(const engine::Window& window) = 0;
	public:
		inline const unsigned int& getHealth() const { return _health; }
		inline const float& getMovementSpeed() const { return _movementSpeed; }
		inline const State& getState() const { return _state; }
		Weapon* getWeapon();

		void setHealth(unsigned int value);
		void setMovementSpeed(float value);
		void setSight(float sightAngle, float sightRange);
		void setState(const State& state);
		void setWeapon(Weapon* weapon);
		virtual void setOnSightFunction(void (*foo)(GameObject*));

		void addSighted(GameObject* sighted);
		void addPickupable(Pickup* pickupable);
		void clearPickupable();

		bool objectIsInSight(const GameObject& gameObject);

		inline const math::Vector2& getMoveToPoint() const { return _moveToPoint; }
		virtual void setMoveToPoint(const math::Vector2& point);

		inline const math::Vector2& getMoveToCheckPoint() const { return _moveToCheckPoint; }
		virtual void setMoveToCheckPoint(const math::Vector2& point);
	};
}