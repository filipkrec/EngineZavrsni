#include "Hitbox.h"
#include <algorithm>

namespace objects {
	Hitbox::Hitbox()
	{
	}


	Hitbox::~Hitbox()
	{
	}

	Hitbox::Hitbox(const Hitbox& other)
		:_shape(other._shape),_collisionRange(other._collisionRange),_boundSprite(other._boundSprite)
	{
		_boundSprite->DoNotDestroySprite();
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

	bool Hitbox::isHit(const Hitbox& other) const
	{
		math::Vector2 A = other.getSpritePosition() - other.getCollisionRange();
		math::Vector2 B = other.getSpritePosition() + math::Vector2(other.getCollisionRange().x, -other.getCollisionRange().y);
		math::Vector2 C = other.getSpritePosition() + other.getCollisionRange();
		math::Vector2 D = other.getSpritePosition() + math::Vector2(-other.getCollisionRange().x, other.getCollisionRange().y);

		math::Vector2 E = getSpritePosition() - getCollisionRange();
		math::Vector2 F = getSpritePosition() + math::Vector2(getCollisionRange().x, -getCollisionRange().y);
		math::Vector2 G = getSpritePosition() + getCollisionRange();
		math::Vector2 H = getSpritePosition() + math::Vector2(-getCollisionRange().x, getCollisionRange().y);
		switch (_shape) {
		case(SQUARE):
			if (
				(A >= E && A <= G) ||
				(B >= E && B <= G) ||
				(C >= E && C <= G) ||
				(D >= E && D <= G) ||
				(E >= A && E <= C) ||
				(F >= A && F <= C) ||
				(G >= A && G <= C) ||
				(H >= A && H <= C)
				)
				return true;
			break;
		default: break;
		}
	}

	bool Hitbox::willBeHit(const Hitbox & other, const math::Vector2 nextMove) const
	{
		math::Vector2 A = other.getSpritePosition() - other.getCollisionRange();
		math::Vector2 B = other.getSpritePosition() + math::Vector2(other.getCollisionRange().x, -other.getCollisionRange().y);
		math::Vector2 C = other.getSpritePosition() + other.getCollisionRange();
		math::Vector2 D = other.getSpritePosition() + math::Vector2(-other.getCollisionRange().x, other.getCollisionRange().y);

		math::Vector2 E = getSpritePosition() - getCollisionRange() + nextMove;
		math::Vector2 F = getSpritePosition() + math::Vector2(getCollisionRange().x, -getCollisionRange().y) + nextMove;
		math::Vector2 G = getSpritePosition() + getCollisionRange() + nextMove;
		math::Vector2 H = getSpritePosition() + math::Vector2(-getCollisionRange().x, getCollisionRange().y) + nextMove;
		switch (_shape) {
		case(SQUARE):
			if (
				(A >= E && A <= G) ||
				(B >= E && B <= G) ||
				(C >= E && C <= G) ||
				(D >= E && D <= G) ||
				(E >= A && E <= C) ||
				(F >= A && F <= C) ||
				(G >= A && G <= C) ||
				(H >= A && H <= C)
				)
				return true;
			break;
		default: break;
		}
		return false;
	}


	math::Vector4 Hitbox::getLineIntersection(const math::Vector2& vectorOrigin, const math::Vector2& vectorEndpoint)
	{

			math::Vector2 min = _boundSprite->getPosition() - _collisionRange;
			math::Vector2 max = _boundSprite->getPosition() + _collisionRange;

			float ymin = min.y;
			float ymax = max.y;
			float xmin = min.x;
			float xmax = max.x;

			float x0 = vectorOrigin.x;
			float x1 = vectorEndpoint.x;
			float y0 = vectorOrigin.y;
			float y1 = vectorEndpoint.y;
			
			int outcode0 = computeCode(vectorOrigin);
			int outcode1 = computeCode(vectorEndpoint);
			bool accept = false;

			while (true) {
				if (!(outcode0 | outcode1)) {
					// bitwise OR is 0: both points inside window; trivially accept and exit loop
					accept = true;
					break;
				}
				else if (outcode0 & outcode1) {
					// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
					// or BOTTOM), so both must be outside window; exit loop (accept is false)
					break;
				}
				else {
					// failed both tests, so calculate the line segment to clip
					// from an outside point to an intersection with clip edge
					float x, y;

					// At least one endpoint is outside the clip rectangle; pick it.
					int outcodeOut = outcode0 ? outcode0 : outcode1;

					// Now find the intersection point;
					// use formulas:
					//   slope = (y1 - y0) / (x1 - x0)
					//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
					//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
					// No need to worry about divide-by-zero because, in each case, the
					// outcode bit being tested guarantees the denominator is non-zero
					if (outcodeOut & 8) {           // point is above the clip window
						x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
						y = ymax;
					}
					else if (outcodeOut & 4) { // point is below the clip window
						x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
						y = ymin;
					}
					else if (outcodeOut & 2) {  // point is to the right of clip window
						y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
						x = xmax;
					}
					else if (outcodeOut & 1) {   // point is to the left of clip window
						y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
						x = xmin;
					}

					// Now we move outside point to intersection point to clip
					// and get ready for next pass.
					if (outcodeOut == outcode0) {
						x0 = x;
						y0 = y;
						outcode0 = computeCode(math::Vector2(x0, y0));
					}
					else {
						x1 = x;
						y1 = y;
						outcode1 = computeCode(math::Vector2(x1, y1));
					}
				}
			}
			if (accept)
				return math::Vector4(x0, y0, x1, y1);
			else
				return math::Vector4(0, 0, 0, 0);
		}

		int Hitbox::computeCode(const math::Vector2& vector)
		{
			math::Vector2 min = _boundSprite->getPosition() - _collisionRange;
			math::Vector2 max = _boundSprite->getPosition() + _collisionRange;

			float ymin = min.y;
			float ymax = max.y;
			float xmin = min.x;
			float xmax = max.x;

			int code;
			code = 0;          // initialised as being inside of [[clip window]]

			if (vector.x < xmin)           // to the left of clip window
				code |= 1;
			else if (vector.x > xmax)      // to the right of clip window
				code |= 2;
			if (vector.y < ymin)           // below the clip window
				code |= 4;
			else if (vector.y > ymax)      // above the clip window
				code |= 8;

			return code;
		}
}