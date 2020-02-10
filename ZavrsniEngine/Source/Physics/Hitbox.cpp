#include "Hitbox.h"

namespace physics {
	Hitbox::Hitbox()
	{
	}


	Hitbox::~Hitbox()
	{
	}

	Hitbox::Hitbox(graphics::Sprite* sprite)
		:_boundSprite(sprite),_shape(Shape::SQUARE)
	{
		math::Vector2 center = math::Vector2(0.0f, 0.0f);
		_collisionRange = math::Vector2(center.x + sprite->getSize().x/2, center.y + sprite->getSize().y / 2);
	}

	Hitbox::Hitbox(graphics::Sprite* sprite, Shape shape, const float width, const float height) :
		_boundSprite(sprite), _shape(shape)
	{
		math::Vector2 center = math::Vector2(0.0f, 0.0f);
		float widthFinal = width / 2;
		float heightFinal;
		if (height == 0)
			heightFinal = widthFinal;
		else
			heightFinal = height / 2;
		switch (shape) {
		case(CIRCLE): break;
		case(SQUARE):
			_collisionRange = math::Vector2(center.x + widthFinal, center.y + heightFinal);
			break;
		case(CUSTOM): break;
		default: break;
		}
	}


	Hitbox::Hitbox(graphics::Sprite* sprite, Shape shape, math::Vector2 collisionRange)
		:_boundSprite(sprite),_shape(shape), _collisionRange(collisionRange)
	{

	}

	bool Hitbox::isHit(const math::Vector2& point) const
	{
		math::Vector2 collisionRangeFinal[2];
		collisionRangeFinal[0] = _boundSprite->getPosition() - _collisionRange;
		collisionRangeFinal[1] = _boundSprite->getPosition() + _collisionRange;
		switch (_shape) {
		case(SQUARE):
			if (
				point.x >= collisionRangeFinal[0].x && point.x <= collisionRangeFinal[1].x &&
				point.y >= collisionRangeFinal[0].y && point.y <= collisionRangeFinal[1].y
				)
				return true;
			break;
		default: break;
		}
		return false;
	}

	bool Hitbox::isHit(const Hitbox& other) const
	{
		math::Vector2 collisionRangeFinal[2];
		collisionRangeFinal[0] = _boundSprite->getPosition() - _collisionRange;
		collisionRangeFinal[1] = _boundSprite->getPosition() + _collisionRange;

		math::Vector2 otherCollisionRangeFinal[2];
		otherCollisionRangeFinal[0] = other.getSpritePosition() - other.getCollisionRange();
		otherCollisionRangeFinal[1] = other.getSpritePosition() + other.getCollisionRange();
		switch (_shape) {
		case(SQUARE):
			if (
				(otherCollisionRangeFinal[0].x >= collisionRangeFinal[0].x && otherCollisionRangeFinal[0].x <= collisionRangeFinal[1].x ||
				otherCollisionRangeFinal[1].x >= collisionRangeFinal[0].x && otherCollisionRangeFinal[1].x <= collisionRangeFinal[1].x) &&
				(otherCollisionRangeFinal[0].y >= collisionRangeFinal[0].y && otherCollisionRangeFinal[0].y <= collisionRangeFinal[1].y ||
					otherCollisionRangeFinal[1].y >= collisionRangeFinal[0].y && otherCollisionRangeFinal[1].y <= collisionRangeFinal[1].y)
				)
				return true;
			break;
		default: break;
		}
		return false;
	}
}