#pragma once
#include "Actor.h"
#include "Inventory/Weapon.h"

namespace objects {
		Actor::Actor() : GameObject() {}

		Actor::Actor(GameObject& gameObject, unsigned int health, float movementSpeed, const ActorState& state)
			: GameObject(gameObject), _health(health), _movementSpeed(movementSpeed), _state(state), _weapon(nullptr),_actorTimer(engine::Timer()),
		_pointReached(false), _seekCheckpoint(false), _patrol(false), _patroling(false), _onSight(nullptr),_sightAngle(0),_sightRange(0),_animationTimer(0)
		{
			_allTextures.push_back((std::make_pair(gameObject.getSprite()->getTexture(), state)));
			setState(state);
		}

		void Actor::init()
		{
			if(_boundSprite != nullptr)
			_boundSprite->DoNotDestroySprite();
		}
		
		void Actor::addTexture(const graphics::Texture* texture, ActorState state)
		{ 
			_allTextures.push_back(std::make_pair(texture , state));
			if (_state == state)
			{
				setState(state);
			}
		}
		void Actor::setAnimationTimerForState(float interval, ActorState state) 
		{
			
			std::vector<std::pair<float, ActorState>>::iterator it
				= std::find_if(_animationTimers.begin(), _animationTimers.end(), [&](const std::pair<float, ActorState>& x)
					{
						return x.second == state;
					});
			if (it != _animationTimers.end() || _animationTimers.empty())
				_animationTimers.push_back(std::make_pair(interval, state));
			else
			{
				(*it).first = interval;
			}
			
			if (_state == state)
			{
				setState(state);
			}
		}
		
		void Actor::moveWeapon()
		{
			if (_weapon != nullptr)
			{
				_weapon->setPosition(_boundSprite->getPosition() + math::Matrix4::rotation(_boundSprite->getRotation(),math::Vector3(0,0,1)) * _weaponOffset);
				_weapon->setScale(_boundSprite->getScale());
				_weapon->setRotation(_boundSprite->getRotation());
			}
		}

		void Actor::move()
		{
			if (_state != ActorState::STATE_DEAD)
			{	
				if (_currentForce.z != 0 && _state != ActorState::STATE_MOVING)
					setState(ActorState::STATE_MOVING);
				else if (_currentForce.z == 0 && _state != ActorState::STATE_STILL)
					setState(ActorState::STATE_STILL);

				GameObject::move();
				moveWeapon();
			}
		}

		void Actor::pickup(Pickup& pickup)
		{
			pickup.onPickup(*this);
		}

		void Actor::animate()
		{
			if (_stateTextures.size() > 1 && _animationTimer.elapsed() >= _animationTime)
			{
				_animationTimer.reset();
				const graphics::Texture* temp = _stateTextures.front();
				_stateTextures.erase(_stateTextures.begin());
				_stateTextures.push_back(temp);
				_boundSprite->swapTexture(_stateTextures.at(0));
			}
		}

		Weapon* Actor::getWeapon() 
		{ 
			return _weapon; 
		}

		void Actor::setHealth(unsigned int value)
		{ 
			_health = value; 
			if (_health <= 0)
			{
				setState(ActorState::STATE_DEAD);
			}
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

		void Actor::setState(const ActorState& state)
		{ 
			_state = state; 

			//textures
			_stateTextures.clear();
			for (std::pair<const graphics::Texture*, ActorState> texture : _allTextures)
			{
				if (texture.second == state)
				{
					_stateTextures.push_back(texture.first);
				}
			}

			if (!_stateTextures.empty())
			{
				_boundSprite->swapTexture(_stateTextures.at(0));
			}

			//animationTimer
			std::vector<std::pair<float, ActorState>>::iterator it
				= std::find_if(_animationTimers.begin(), _animationTimers.end(), [&](const std::pair<float, ActorState>& x)
					{
						return x.second == state;
					});
			if (it != _animationTimers.end())
				_animationTime = (*it).first;
			else
				_animationTime = 0.0f;

			_animationTimer.reset();
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

		void Actor::onHit(const objects::Weapon* weapon)
		{
			_health -= rand() % (weapon->getDmgMax() - weapon->getDmgMin());
			weapon->onShot((GameObject*)this);
		}


		void Actor::setWeaponOffset(const math::Vector2& weaponOffset)
		{
			_weaponOffset = weaponOffset;
		}

		bool Actor::objectIsInSight(const GameObject& gameObject)
		{
			if (_state != ActorState::STATE_DEAD)
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
			}
			return false;
		}

		void Actor::addSighted(GameObject* sighted)
		{
			if (_state != ActorState::STATE_DEAD)
			{
				_sighted.push_back(sighted);
			}
		}

		void Actor::clearObstructedSighted()
		{
			bool obstructed = false;
			math::Vector2 points[4];
			for (GameObject* sighted : _sighted)
			{
				points[0] = sighted->getSpritePosition() - sighted->getCollisionRange();
				points[1] = sighted->getSpritePosition() + math::Vector2(sighted->getCollisionRange().x, -sighted->getCollisionRange().y);
				points[2] = sighted->getSpritePosition() + sighted->getCollisionRange();
				points[3] = sighted->getSpritePosition() + math::Vector2(-sighted->getCollisionRange().x, sighted->getCollisionRange().y);

				for (GameObject* otherSighted : _sighted)
				{
					if (otherSighted == sighted)
						continue;

					obstructed = true;
					for (int i = 0; i < 4; ++i)
					{
						if (otherSighted->getLineIntersection(points[i], getSpritePosition()) == math::Vector4(0, 0, 0, 0))
						{
							obstructed = false;
							break;
						}
					}
					if (obstructed == true)
						break;
				}

				if (obstructed == true)
				{
					_sighted.erase(std::find_if(_sighted.begin(), _sighted.end(), [&](GameObject* x) {return x == sighted; }));
				}
			}
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


		void Actor::collide(GameObject& other)
		{
			if (_state != ActorState::STATE_DEAD)
			{
				GameObject::collide(other);
				if (_seekCheckpoint == false)
					toggleSeekCheckpoint();
			}
		}

		void Actor::setMoveToPoint(const math::Vector2& point)
		{
			_moveToPoint = point;

			if (_pointReached == true)
				togglePointReached();

			if (_seekCheckpoint == false)
				toggleSeekCheckpoint();
		}


		void Actor::setMoveToCheckPoint(const math::Vector2& point)
		{
			_moveToCheckPoint = point;
		}

		void Actor::setPatrolOriginPoint(const math::Vector2& point)
		{
			_patrolOriginPoint = point;
		}

		void Actor::setPatrolEndPoint(const math::Vector2& point)
		{
			_patrolEndPoint = point;
		}

		void Actor::resetActorTimer()
		{
			_actorTimer.reset();
		}

		void Actor::toggleSeekCheckpoint()
		{
			_seekCheckpoint = !_seekCheckpoint;
		}
		void Actor::togglePointReached()
		{
			_pointReached = !_pointReached;
		}
		void Actor::togglePatrol()
		{
			_patrol = !_patrol;
		}
		void Actor::togglePatroling()
		{
			_patroling = !_patroling;
		}
}