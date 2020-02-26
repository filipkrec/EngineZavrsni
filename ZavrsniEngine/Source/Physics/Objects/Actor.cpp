#pragma once
#include "Actor.h"
#include "Inventory/Weapon.h"

namespace objects {
		Actor::Actor() {}

		Actor::Actor(GameObject& gameObject, unsigned int health, float movementSpeed, const State& state)
			: GameObject(gameObject), _health(health), _movementSpeed(movementSpeed), _state(state), _weapon(nullptr),_actorTimer(engine::Timer()) {}
		void Actor::addTexture(const graphics::Texture& texture) { _textureIds.push_back(texture.getId()); }
		void Actor::moveWeapon()
		{
			if (_weapon != nullptr)
			{
				_weapon->setPosition(_boundSprite->getPosition());
				_weapon->setScale(_boundSprite->getScale());
				_weapon->setRotation(_boundSprite->getRotation());
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

		void Actor::setSight(float sightAngle, float sightRange)
		{
			_sightAngle = sightAngle;
			_sightRange = sightRange;
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

		void Actor::rotateToPoint(math::Vector2 point)
		{
			float rotation = math::Vector2::getAngleBetween(_boundSprite->getRotation(), point);

			if (rotation < 0)
			{
				if (rotation <= -5.0f)
					_boundSprite->rotate(-5.0f);
				else
					_boundSprite->rotate(rotation);

			}
			else if (rotation > 0)
			{
				if (rotation >= 5.0f)
					_boundSprite->rotate(5.0f);
				else
					_boundSprite->rotate(rotation);
			}
		}


		bool Actor::objectIsInSight(const GameObject& gameObject)
		{
			if (_sightAngle == 0 && _sightRange == 0)
				return true;

			math::Vector2 objectPosition = gameObject.getSpritePosition();
			float objectWidth = gameObject.getSpriteSize().x;
			float objectHeight = gameObject.getSpriteSize().y;
			math::Vector2 objectPoints[4] =
			{
				math::Vector2(objectPosition.x - objectWidth,objectPosition.y - objectHeight),
				math::Vector2(objectPosition.x + objectWidth,objectPosition.y - objectHeight),
				math::Vector2(objectPosition.x + objectWidth,objectPosition.y + objectHeight),
				math::Vector2(objectPosition.x - objectWidth,objectPosition.y + objectHeight)
			};

			math::Vector2 spritePosition = _boundSprite->getPosition();
			for (int i = 0; i < 4; ++i)
			{
				if (spritePosition.distanceFrom(objectPoints[i]) <= _sightRange || _sightRange == 0)
				{
					float angle = math::Vector2::getAngleBetween(_boundSprite->getRotation(), objectPoints[i] - _boundSprite->getPosition());
						if (_sightAngle / 2 >= abs(angle) || _sightAngle == 0)
							return true;
				}
			}
			return false;
		}

		void Actor::addSighted(GameObject* sighted)
		{
			_sighted.push_back(sighted);
		}

		void Actor::processSight()
		{
			if (_onSight != nullptr)
				for (GameObject* sightedObject : _sighted)
				{
					_onSight(sightedObject);
				}

			_sighted.clear();
		}


		void Actor::setOnSightFunction(void (*foo)(GameObject*))
		{
			_onSight = foo;
		}

		void Actor::setMoveToPoint(const math::Vector2& point)
		{
			_moveToPoint = point;
		}


		void Actor::setMoveToCheckPoint(const math::Vector2& point)
		{
			_moveToCheckPoint = point;
		}

		void Actor::resetActorTimer()
		{
			_actorTimer.reset();
		}
}