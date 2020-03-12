#include "GameObject.h"
namespace objects {

	GameObject::GameObject()
		:Hitbox()
	{
	}


	GameObject::~GameObject()
	{
	}

	GameObject::GameObject(const graphics::Sprite& sprite)
		: Hitbox(sprite), _colissionOn(false), _allegiance(Allegiance::NEUTRAL)
	{
	}

	GameObject::GameObject(const graphics::Sprite& sprite, unsigned int weight, bool enviroment)
		: Hitbox(sprite), _weight(weight == 0 ? WEIGHTMAX : weight),_colissionOn(true), _allegiance(enviroment ? Allegiance::ENVIROMENT :Allegiance::NEUTRAL)
	{
		
	}

	GameObject::GameObject(const graphics::Sprite& sprite, unsigned int weight, Shape shape, float width, float height)
		: Hitbox(sprite, shape, width, height), _weight(weight == 0 ? WEIGHTMAX : weight), _colissionOn(true), _allegiance(Allegiance::NEUTRAL)
	{
	}


	void GameObject::calculateColission(const math::Vector3& force)
	{
		if (_currentForce.x == 0 && _currentForce.y == 0)
		{
			_currentForce.x = force.x;
			_currentForce.y = force.y;
			_currentForce.z = force.z;
		}
		else
		{
			math::Vector3 tempForce = force;

			_currentForce.x *= _currentForce.z;
			_currentForce.y *= _currentForce.z;

			tempForce.x *= tempForce.z;
			tempForce.y *= tempForce.z;

			_currentForce.x += tempForce.x;
			_currentForce.y += tempForce.y;
			_currentForce.z = (abs(_currentForce.x) + abs(_currentForce.y)) / 2;

			math::Vector2 unitVector = math::Vector2::calculateUnitVector(math::Vector2(_currentForce.x,_currentForce.y));
			_currentForce.x = unitVector.x;
			_currentForce.y = unitVector.y;
		}
	}


	void GameObject::collide(GameObject& other)
	{
		if (_colissionOn)
		{
			if (other._previousForce.z > 0)
			{
				other.calculateColission(math::Vector3(-other._previousForce.x, -other._previousForce.y, other._previousForce.z));
				calculateColission(math::Vector3(other._previousForce.x, other._previousForce.y, other._previousForce.z));
				calculateNextMove();
				other.calculateNextMove();
			}

			if (_previousForce.z > 0)
			{
				other.calculateColission(math::Vector3(_previousForce.x, _previousForce.y, _previousForce.z));
				calculateColission(math::Vector3(-_previousForce.x, -_previousForce.y, _previousForce.z));
				calculateNextMove();
				other.calculateNextMove();
			}

			if (_previousForce.z == 0 && other._previousForce.z == 0)
			{
				math::Vector2 vector = getPosition() - other.getPosition();
				float multiplier = getPosition().distanceFrom(_collisionRange) / getPosition().distanceFrom(other.getPosition());
				vector = math::Vector2::calculateUnitVector(vector);
				if (_weight / other._weight <= 2)
				{
					other.calculateColission(math::Vector3(-vector.x, -vector.y, _weight * FRICTION * multiplier));
					calculateColission(math::Vector3(vector.x, vector.y, _weight * FRICTION * multiplier));
				}
				else
				{
					other.calculateColission(math::Vector3(-vector.x, -vector.y, 2 * other._weight * FRICTION * multiplier));
					calculateColission(math::Vector3(vector.x, vector.y, 2 * other._weight * FRICTION * multiplier));
				}
				calculateNextMove();
				other.calculateNextMove();
			}


			_previousForce.x = 0;
			_previousForce.y = 0;
			_previousForce.z = 0;


			other._previousForce.x = 0;
			other._previousForce.y = 0;
			other._previousForce.z = 0;
		}
	}

	void GameObject::move()
	{
		_currentSpeed = (_currentForce.z / _weight) / PROCESSING_INTERVAL;
		_currentForce.z -= (_weight * 10 * FRICTION) / PROCESSING_INTERVAL;
		if (_currentForce.z > 0)
			movePosition(math::Vector2(_currentSpeed * _currentForce.x, _currentSpeed * _currentForce.y));
		else
		{
			_currentForce.z = 0;
			_currentForce.x = 0;
			_currentForce.y = 0;
		}
	}

	void GameObject::savePreviousForce()
	{
			_previousForce.x = _currentForce.x;
			_previousForce.y = _currentForce.y;
			_previousForce.z = _currentForce.z;
	}

	void GameObject::calculateNextMove()
	{
		_currentSpeed = (_currentForce.z / _weight) / PROCESSING_INTERVAL;
		_currentForce.z -= (_weight * 10 * FRICTION) / PROCESSING_INTERVAL;
		if (_currentForce.z > 0)
			_nextMove = math::Vector2(_currentSpeed * _currentForce.x, _currentSpeed * _currentForce.y);
		else
			_currentForce.z = 0;
	}

	void GameObject::setAllegiance(Allegiance allegiance)
	{
		_allegiance = allegiance;
	}

	void GameObject::toggleColission()
	{
		_colissionOn = !_colissionOn;
	}
}