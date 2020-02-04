#include "Hitbox.h"

namespace physics {
	Hitbox::Hitbox(graphics::Sprite const* sprite, Shape shape, const float width, const float height) :
		_boundSprite(sprite), _shape(shape)
	{
		//math::Vector2 spriteSize = sprite->getSize();
		//math::Vector2 center = math::Vector2(spriteSize.x / 2, spriteSize.y / 2);
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

	Hitbox::~Hitbox()
	{
	}

	bool Hitbox::isHit(const math::Vector2& point) const
	{
		math::Vector2 spriteCenter((_boundSprite->getSize() / 2) + _boundSprite->getPosition()[0]);
		math::Vector2 collisionRangeFinal[2];
		collisionRangeFinal[0] = spriteCenter - _collisionRange;
		collisionRangeFinal[1] = spriteCenter + _collisionRange;
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
		math::Vector2 spriteCenter((getSpriteSize() / 2) + getSpritePosition()[0]);
		math::Vector2 collisionRangeFinal[2];
		collisionRangeFinal[0] = spriteCenter - _collisionRange;
		collisionRangeFinal[1] = spriteCenter + _collisionRange;

		math::Vector2 otherSpriteCenter((other.getSpriteSize() / 2) + other.getSpritePosition()[0]);
		math::Vector2 otherCollisionRangeFinal[2];
		otherCollisionRangeFinal[0] = otherSpriteCenter - other.getCollisionRange();
		otherCollisionRangeFinal[1] = otherSpriteCenter + other.getCollisionRange();
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