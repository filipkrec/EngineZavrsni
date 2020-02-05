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

		
		if (_currentForce.x == 0 && _currentForce.y == 0)
		{
			_currentForce.x = force.x;
			_currentForce.y = force.y;
		}
		else
		{
			float forceDivision = force.z / _currentForce.z;
			_currentForce.x = (_currentForce.x + force.x * forceDivision) / 2;
			_currentForce.y = (_currentForce.y + force.y * forceDivision) / 2;
			math::Vector2 unitVector = unitVector.calculateUnitVector(_currentForce.x, _currentForce.y);
			_currentForce.x = unitVector.x;
			_currentForce.y = unitVector.y;
		}
		_currentForce.z += force.z;
	}

	void GameObject::move()
	{
		if (_currentForce.z >= 1.0f)
		{
			//izvr�ava se 60 puta u sekundi
			float speed = (_currentForce.z / _weight) / 60;
			_currentForce.z -= (_currentForce.z * FRICTION) / 60;
			_sprite->applyTransformation(math::Matrix4::translation(math::Vector2(speed * _currentForce.x, speed * _currentForce.y)));
		}
		else 
		{
			_currentForce.z = 0.0f;
			_currentForce.x = 0.0f;
			_currentForce.y = 0.0f;
		}
	}
}