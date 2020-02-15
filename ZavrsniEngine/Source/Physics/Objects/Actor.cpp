#pragma once
#include "Actor.h"
#include "Inventory/Weapon.h"

namespace objects {
		Actor::Actor() {}

		Actor::Actor(GameObject& gameObject, unsigned int health, float movementSpeedMax, const State& state)
			: GameObject(gameObject), _health(health), _movementSpeed(0), _movementSpeedMax(movementSpeedMax), _state(state), _weapon(nullptr) {}
		void Actor::addTexture(const graphics::Texture& texture) { _textureIds.push_back(texture.getId()); }
		void Actor::moveWeapon()
		{
			if (_weapon != nullptr)
			{
				_weapon->setPosition(_boundSprite->getPosition());
				_weapon->setScale(_boundSprite->getScale());
				_weapon->rotate(_boundSprite->getRotation());
			}
		}

		void Actor::move()
		{
			GameObject::move();
			moveWeapon();
		}

		void Actor::pickup(Pickup& pickup)
		{
			pickup.onPickup(*this);
		}

		Weapon* Actor::getWeapon() 
		{ 
			return _weapon; 
		}

		void Actor::setHealth(unsigned int value)
		{ 
			_health = value; 
		}

		void Actor::setMovementSpeed(float value)
		{ 
			_movementSpeed = value; 
		}

		void Actor::setMovementSpeedMax(float value)
		{ 
			_movementSpeedMax = value; 
		}

		void Actor::setState(const State& state)
		{ 
			_state = state; 
		}

		void Actor::setWeapon(Weapon* weapon)
		{
			_weapon = weapon;
		}

		void Actor::addPickupable(Pickup* pickupable)
		{
			_pickUpable.push_back(pickupable);
		}

		void Actor::clearPickupable()
		{
			_pickUpable.clear();
		}
}