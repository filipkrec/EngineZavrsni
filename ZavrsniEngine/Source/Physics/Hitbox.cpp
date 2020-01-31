#include "Hitbox.h"

namespace physics {
	Hitbox::Hitbox(graphics::Sprite* sprite, Shape shape, const float& width, const float& height) :
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

	bool Hitbox::isHit(const math::Vector2& point)
	{
		math::Vector2 spriteCenter((_boundSprite->getSize() / 2) + _boundSprite->getPosition()[0]);
		math::Vector2 _collisionRangeFinal[2];
		_collisionRangeFinal[0] = spriteCenter - _collisionRange;
		_collisionRangeFinal[1] = spriteCenter + _collisionRange;
		switch (_shape) {
		case(SQUARE):
			if (
				point.x >= _collisionRangeFinal[0].x && point.x <= _collisionRangeFinal[1].x &&
				point.y >= _collisionRangeFinal[0].y && point.y <= _collisionRangeFinal[1].y
				)
				return true;
			break;
		default: break;
		}
		return false;
	}
}