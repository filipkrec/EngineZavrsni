#pragma once
#include "Actor.h"
#include "Inventory/Weapon.h"

namespace objects {
		Actor::Actor() : GameObject() {}

		Actor::Actor(GameObject& gameObject, unsigned int health, float movementSpeed, const ActorState& state)
			: GameObject(gameObject), _health(health), _maxHealth(health), _movementSpeed(movementSpeed), _state(state), _weapon(nullptr),_actorTimer(engine::Timer()),
		_pointReached(false), _seekPath(false), _pathFinished(false), _patrol(false), _patroling(false), _onSight(nullptr),_sightAngle(0),_sightRange(0),_animationTimer(), _switchedWeapon(false)
		{
			_allTextures.push_back((std::make_pair(gameObject.getTexture(), state)));
			setState(state);
		}

		void Actor::init()
		{
			DoNotDestroySprite();
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
				_weapon->setPosition(getPosition() + math::Matrix4::rotation(getRotation(),math::Vector3(0,0,1)) * _weaponOffset);
				_weapon->setScale(getScale());
				_weapon->setRotation(getRotation());
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
				swapTexture(_stateTextures.at(0));
			}
		}

		Weapon* Actor::getWeapon() 
		{ 
			return _weapon; 
		}

		void Actor::setHealth(int value)
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
			if (state == ActorState::STATE_DEAD)
			{
				_colissionOn = false;
				if (_weapon != nullptr)
				{
					_weapon->DestroySprite();
					_weapon = nullptr;
				}
			}

			for (std::pair<const graphics::Texture*, ActorState> texture : _allTextures)
			{
				if (texture.second == state)
				{
					_stateTextures.push_back(texture.first);
				}
			}

			if (!_stateTextures.empty())
			{
				swapTexture(_stateTextures.at(0));
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
			_switchedWeapon = true;
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
			float rotation = math::Vector2::getAngleBetween(getRotation(), point);

			if (rotation < 0)
			{
				if (rotation <= -5.0f)
					rotate(-5.0f);
				else
					rotate(rotation);

			}
			else if (rotation > 0)
			{
				if (rotation >= 5.0f)
					rotate(5.0f);
				else
					rotate(rotation);
			}
		}

		void Actor::onHit(const objects::Weapon* weapon)
		{
			unsigned int damage = weapon->getDmgMax() - weapon->getDmgMin() == 0 ? weapon->getDmgMax() : weapon->getDmgMax() - rand() % (weapon->getDmgMax() - weapon->getDmgMin());
			setHealth(_health - damage);
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

				if (gameObject.getPosition().distanceFrom(getPosition()) - gameObject.getCollisionRange().length() > _sightRange)
					return false;

				math::Vector2 objectPosition = gameObject.getPosition();
				float objectWidth = gameObject.getSize().x;
				float objectHeight = gameObject.getSize().y;
				math::Vector2 objectPoints[4] =
				{
					math::Vector2(objectPosition.x - objectWidth,objectPosition.y - objectHeight),
					math::Vector2(objectPosition.x + objectWidth,objectPosition.y - objectHeight),
					math::Vector2(objectPosition.x + objectWidth,objectPosition.y + objectHeight),
					math::Vector2(objectPosition.x - objectWidth,objectPosition.y + objectHeight)
				};

				math::Vector2 spritePosition = getPosition();
				for (int i = 0; i < 4; ++i)
				{
					if (spritePosition.distanceFrom(objectPoints[i]) <= _sightRange || _sightRange == 0)
					{
						float angle = math::Vector2::getAngleBetween(getRotation(), objectPoints[i] - getPosition());
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
				points[0] = sighted->getPosition() - sighted->getCollisionRange();
				points[1] = sighted->getPosition() + math::Vector2(sighted->getCollisionRange().x, -sighted->getCollisionRange().y);
				points[2] = sighted->getPosition() + sighted->getCollisionRange();
				points[3] = sighted->getPosition() + math::Vector2(-sighted->getCollisionRange().x, sighted->getCollisionRange().y);

				for (GameObject* otherSighted : _sighted)
				{
					if (otherSighted == sighted)
						continue;

					obstructed = true;
					for (int i = 0; i < 4; ++i)
					{
						if (otherSighted->getLineIntersection(points[i], getPosition()) == math::Vector4(0, 0, 0, 0))
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
					_sighted.erase(std::remove_if(_sighted.begin(), _sighted.end(), [&](GameObject* x) {return x == sighted; }),_sighted.end());
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

				if (other.getAllegiance() != Allegiance::ENVIROMENT && _seekPath == false)
					toggleSeekPath();
			}
		}

		void Actor::setMoveToPoint(const math::Vector2& point)
		{
			_moveToPoint = point;

			if (_pointReached == true)
				togglePointReached();

			if (_seekPath == false)
				toggleSeekPath();
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

		void Actor::clearPath()
		{
			_path.clear();
		}

		void Actor::pushPath(const math::Vector2& pathPoint)
		{
			_path.push_back(pathPoint);
		}


		void Actor::popPath()
		{
			if (!_path.empty())
			{
				_path.pop_back();
				if (_path.empty() && !seekPath())
					toggleSeekPath();
			}


		}

		void Actor::toggleSeekPath()
		{
			_seekPath = !_seekPath;
			if (_seekPath == true)
				clearPath();
		}

		void Actor::togglePathFinished()
		{
			_pathFinished = !_pathFinished;
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
		void Actor::toggleSwitchedWeapon()
		{
			_switchedWeapon = !_switchedWeapon;
		}
}