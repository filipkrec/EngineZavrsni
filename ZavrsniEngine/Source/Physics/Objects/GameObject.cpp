#include "GameObject.h"
namespace physics {

	GameObject::GameObject()
	{
	}


	GameObject::~GameObject()
	{
	}

	GameObject::GameObject(graphics::Sprite* sprite, unsigned int weight)
		: Hitbox(sprite), _weight(weight)
	{
	}

	GameObject::GameObject(graphics::Sprite* sprite, unsigned int weight, Shape shape, float width, float height)
		:Hitbox(sprite,shape,width,height),_weight(weight)
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
			
			math::Vector2 unitVector = unitVector.calculateUnitVector(_currentForce.x, _currentForce.y);
			_currentForce.x = unitVector.x;
			_currentForce.y = unitVector.y;
		}
	}


	void GameObject::collide(GameObject& other)
	{
		if (other._currentForce.z == 0)
		{
			calculateColission(math::Vector3(_previousForce.x, _previousForce.y, _weight/ PROCESSING_INTERVAL));
		}
		else calculateColission(math::Vector3(other._previousForce.x, other._previousForce.y, other._previousForce.z / PROCESSING_INTERVAL));
	}

	void GameObject::move()
	{
		if (_currentForce.z >= 1.0f)
		{
			//podjeljeno sa N jer se procesira N puta u sekundi
			float speed = (_currentForce.z / _weight) / PROCESSING_INTERVAL;
			_currentForce.z -= (_currentForce.z * FRICTION) / PROCESSING_INTERVAL;
			_boundSprite->move(math::Vector2(speed * _currentForce.x, speed * _currentForce.y));
		}
		else 
		{
			_currentForce.z = 0.0f;
			_currentForce.x = 0.0f;
			_currentForce.y = 0.0f;
		}
	}

	void GameObject::savePreviousForce()
	{
		_previousForce.x = _currentForce.x;
		_previousForce.y = _currentForce.y;
		_previousForce.z = _currentForce.z;
	}
}