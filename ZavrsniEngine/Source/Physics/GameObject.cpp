#include "GameObject.h"
namespace physics {
	GameObject::GameObject(graphics::Sprite* sprite, unsigned int weight, Hitbox* hitbox)
		:_sprite(sprite), _weight(weight), _hitbox(hitbox)
	{
		if (hitbox == nullptr)
		{
			_hitbox = new Hitbox(sprite, Shape::SQUARE, sprite->getSize().x, sprite->getSize().y);
		}
	}


	bool GameObject::isHit(const Hitbox& other)
	{
		return _hitbox->isHit(other);
	}

	bool GameObject::isHit(const math::Vector2& point)
	{
		return _hitbox->isHit(point);
	}


	void GameObject::calculateColission(const math::Vector3& force)
	{
		const math::Vector2 currentPos = *(_sprite->getPosition());

		float forceDivision = force.z / _currentForce.z;
		
		if (_currentForce.x == 0 && _currentForce.y == 0)
		{
			_currentForce.x = force.x;
			_currentForce.y = force.y;
		}
		else
		{
			//TODO
			_currentForce.x = (_currentForce.x + force.x) / 2;
			_currentForce.y = (_currentForce.y + force.y) / 2;
		}
		_currentForce.z += force.z;
	}

	void GameObject::move()
	{
		float speed = _currentForce.z / _weight - (_currentForce.z * FRICTION) / _weight;
		_currentForce.z -= _currentForce.z * FRICTION;
		_sprite->setPosition(math::Vector2(speed * _currentForce.x, speed * _currentForce.y));
		_currentForce.z /=  FRICTION;
		if (speed <= 0.01f)
		{
			_currentForce.z = 0.0f;
			_currentForce.x = 0.0f;
			_currentForce.y = 0.0f;
		}
	}
}