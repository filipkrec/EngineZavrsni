#pragma once
#include "Actor.h"

namespace objects {
		Actor::Actor() {}

		Actor::Actor(GameObject& gameObject, unsigned int health, float movementSpeedMax, const State& state)
			: GameObject(gameObject), _health(health), _movementSpeed(0), _movementSpeedMax(movementSpeedMax), _state(state), _weapon(nullptr) {}
		void Actor::addTexture(const graphics::Texture& texture) { _textureIds.push_back(texture.getId()); }
		void Actor::moveWeapon()
		{
			if (_weapon != nullptr)
			{
				_weapon->getSprite()->setPosition(_boundSprite->getPosition());
				_weapon->getSprite()->setScale(_boundSprite->getScale());
				_weapon->getSprite()->rotate(_boundSprite->getRotation());
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
}