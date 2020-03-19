#include "NPC.h"

namespace objects {
	std::vector<math::Vector2> NPC::directionsAll;

	NPC::NPC()
	{
	}

	NPC::NPC(GameObject gameObject, unsigned int health, float movementSpeed)
		:Actor(gameObject, health, movementSpeed, ActorState::STATE_STILL),_lookingAt(nullptr),_AIState(AIState::AI_STATE_PASSIVE),_NPCTimer(engine::Timer()),_rotationGoal(0),_reactionDelay(0.1f)
	{
	}

	void NPC::process(const engine::Window& window)
	{
		if (_weapon != nullptr)
		{
			_weapon->clearShots();
		}
		if (getState() != ActorState::STATE_DEAD)
		{
			moveInDirection();
			lookAt();
			processAI();
			processSight();
		}
		animate();
	}

	void NPC::processAI()
	{
		if (_rotationGoal < 0)
		{
			if (_rotationGoal <= -5.0f)
			{
				rotate(-5.0f);
				_rotationGoal += 5.0f;
			}
			else
			{
				rotate(_rotationGoal);
				_rotationGoal = 0;
			}

		}
		else if (_rotationGoal > 0)
		{
			if (_rotationGoal >= 5.0f)
			{
				rotate(5.0f);
				_rotationGoal -= 5.0f;
			}
			else
			{
				rotate(_rotationGoal);
				_rotationGoal == 0;
			}
		}

		if (_NPCTimer.elapsed() <= _reactionDelay)
		{
			return;
		}
		_NPCTimer.reset();


		if (_weapon != nullptr)
		{
			if (_weapon->getClipCurrent() == 0 && _weapon->getAmmoCurrent() >= 0)
				_weapon->reload();
		}

		switch (_AIState) {
		case (AIState::AI_STATE_PASSIVE):
				if (!std::any_of(_sighted.begin(), _sighted.end(),  
					[&](GameObject* x)
					{
						return x == _lookingAt;
					}))
				{
					_lookingAt = nullptr;
				}
				break;
		case (AIState::AI_STATE_DEFENSIVE):
			//if no enemy sighted clear
			if (!std::any_of(_sighted.begin(), _sighted.end(),
				[&](GameObject* x)
				{
					return x == _lookingAt;
				}))
			{
				_lookingAt = nullptr;
			}

			//if enemy sighted move to,alert
			for (GameObject* sightedObject : _sighted)
			{
				if (std::any_of(_enemyAllegiances.begin(), _enemyAllegiances.end(), [&](Allegiance x)
					{
						return x == sightedObject->getAllegiance();
					}))
				{
					setLookingAt(sightedObject);
					setMoveToPoint(sightedObject->getPosition());
					setAIState(AIState::AI_STATE_ALERT);
				}
			}
			break;
		case (AIState::AI_STATE_ALERT):
			//if enemy escapes sight move to its last position
			if (_lookingAt != nullptr && (_sighted.empty() || !std::any_of(_sighted.begin(), _sighted.end(),
				[&](GameObject* x)
				{
					return x == _lookingAt;
				})))
			{
				setMoveToPoint(_lookingAt->getPosition());
				_lookingAt = nullptr;
			}
			//if weapon target
			else if (_weapon != nullptr && _lookingAt != nullptr)
			{
				if (getPosition().distanceFrom(_lookingAt->getPosition()) <= 0.7 * _weapon->getRange() && getPosition().distanceFrom(_lookingAt->getPosition()) <= 0.7 * _sightRange);
				{
					setAIState(AIState::AI_STATE_TARGETING);
				}
			}

			//if no enemy in sight look around
			if (_pointReached && !std::any_of(_sighted.begin(), _sighted.end(),
				[&](GameObject* x)
				{
					return x == _lookingAt;
				}))
			{
				if (_NPCTimer.elapsed() >= 20.0f)
				{
					setAIState(AIState::AI_STATE_DEFENSIVE);
					break;
				}

				if (_rotationGoal == 0)
				{
					_rotationGoal = rand() % 360;
					_rotationGoal -= 180;
				}
			}

				//if enemy seen target 
			for (GameObject* sightedObject : _sighted)
			{
				if (std::any_of(_enemyAllegiances.begin(), _enemyAllegiances.end(), [&](Allegiance x)
					{
						return x == sightedObject->getAllegiance();
					}))
				{
					setLookingAt(sightedObject);
				}
			}
			break;
		case (AIState::AI_STATE_TARGETING):
			std::vector<GameObject*>::iterator it = std::find_if(_sighted.begin(), _sighted.end(),
				[&](GameObject* x)
				{
					return x == _lookingAt;
				});

			if (it == _sighted.end() || getPosition().distanceFrom(_lookingAt->getPosition()) <= 0.7 * _weapon->getRange() && getPosition().distanceFrom(_lookingAt->getPosition()) <= 0.7 * _sightRange)
			{
				setAIState(AIState::AI_STATE_ALERT);
				break;
			}
			else
			{
				setMoveToPoint(getPosition());
			}

			//ako vidi metu pucaj
			if (_weapon != nullptr)
			{
				GameObject* temp = *it;
				rotateToPoint(temp->getPosition());
				if (abs(math::Vector2::getAngleBetween(getRotation(), temp->getPosition() - getPosition()))
					<= _weapon->getSpread() && temp->getState() != objects::ActorState::STATE_DEAD)
				{
					_weapon->shoot();
				}
			}

			break;

		}
	}

	void NPC::moveInDirection()
	{
		if (_moveDirection != math::Vector2(0.0f, 0.0f) && _state != ActorState::STATE_DEAD)
		{
			calculateColission(math::Vector3(_moveDirection.x, _moveDirection.y, (_movementSpeed * MOVEMENT_SPEED_COEFFICIENT) * _weight));

			if(_rotationGoal == 0 || _lookingAt == nullptr)
			rotateToPoint(_moveDirection);
		}
	}

	void NPC::lookAt()
	{
		if (_state != ActorState::STATE_DEAD)
		{
			if (_lookingAt != nullptr && !_lookingAt->toDestroySprite())
				rotateToPoint(_lookingAt->getPosition() - getPosition()); //vektor udaljenosti
		}
	}
	 
	void NPC::init()
	{
		Actor::init();
		if (directionsAll.empty())
		{
			math::Vector2 right = math::Vector2(1, 0);
			directionsAll.push_back(right);
			math::Vector2 up = math::Vector2(0, 1);
			directionsAll.push_back(up);
			math::Vector2 left = math::Vector2(-1, 0);
			directionsAll.push_back(left);
			math::Vector2 down = math::Vector2(0, -1);
			directionsAll.push_back(down);
			math::Vector2 botRight = math::Vector2(1, -1);
			directionsAll.push_back(botRight);
			math::Vector2 topRight = math::Vector2(1, 1);
			directionsAll.push_back(topRight);
			math::Vector2 botLeft = math::Vector2(-1, -1);
			directionsAll.push_back(botLeft);
			math::Vector2 topleft = math::Vector2(-1, 1);
			directionsAll.push_back(topleft);
		}
	}

	void NPC::onHit(const Weapon* weapon)
	{
		Actor::onHit(weapon);
		if (_AIState == AIState::AI_STATE_DEFENSIVE)
			setAIState(AIState::AI_STATE_ALERT);
		setMoveToPoint(weapon->getPosition());
		_rotationGoal = math::Vector2::getAngleBetween(_rotation,weapon->getPosition());
	}
	void NPC::addEnemyAllegiance(Allegiance allegiance)
	{
		_enemyAllegiances.push_back(allegiance);
	}

	void NPC::setMoveDirection(const math::Vector2& direction)
	{
		_moveDirection = direction;
	}

	void NPC::setReactionDelay(float delay)
	{
		_reactionDelay = delay;
	}

	void NPC::setLookingAt(graphics::Sprite* sprite)
	{
		_lookingAt = sprite;
	}

	void NPC::setAIState(AIState state)
	{
		_NPCTimer.reset();
		_AIState = state;
	}

}

/*
int main() {
	for (int i = 0; i & lt; sizeof(points) / sizeof(point); ++i)
	{
		curval = points[i].x == 0 ? 0 : points[i].x / points[i].y;
		if (highval& lt; = curval)
		{
			highval = curval;
			highest = points[i];
		}

		if (lowval& gt; = curval)
		{
			lowval = curval;
			lowest = points[i];
		}
	}
}
*/