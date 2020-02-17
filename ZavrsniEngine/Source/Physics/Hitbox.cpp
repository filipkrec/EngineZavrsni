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


	bool Hitbox::isHit(const math::Vector2& vectorOrigin, const math::Vector2& vectorEndpoint) const
	{
		//cohen sutherland
		math::Vector2 collisionRangeFinal[2];
		collisionRangeFinal[0] = _boundSprite->getPosition() - _collisionRange;//bot left 
		collisionRangeFinal[1] = _boundSprite->getPosition() + _collisionRange; //top right


		//find relevant sides 
		if(isHit(vectorOrigin) || isHit(vectorEndpoint))
			return true;

		if ((vectorOrigin.x >= collisionRangeFinal[0].x && vectorOrigin.x <= collisionRangeFinal[1].x) && (vectorEndpoint.x >= collisionRangeFinal[0].x && vectorEndpoint.x <= collisionRangeFinal[1].x))
			return true;

		if ((vectorOrigin.y >= collisionRangeFinal[0].y && vectorOrigin.y <= collisionRangeFinal[1].y) && (vectorEndpoint.y >= collisionRangeFinal[0].y && vectorEndpoint.y <= collisionRangeFinal[1].x))
			return true;

		if ((vectorOrigin.x < collisionRangeFinal[0].x && vectorEndpoint.x < collisionRangeFinal[0].x) || (vectorOrigin.x > collisionRangeFinal[1].x && vectorEndpoint.x > collisionRangeFinal[1].x))
			return false;

		if ((vectorOrigin.y < collisionRangeFinal[0].y && vectorEndpoint.y < collisionRangeFinal[0].y) || (vectorOrigin.y > collisionRangeFinal[1].y && vectorEndpoint.y > collisionRangeFinal[1].y))
			return false;

		return distanceMinimum(vectorOrigin, vectorEndpoint) >= 0;
	}

	float Hitbox::distanceMinimum(const math::Vector2& vectorOrigin, const math::Vector2& vectorEndpoint) const
	{
		math::Vector2 collisionRangeFinal[2];
		collisionRangeFinal[0] = _boundSprite->getPosition() - _collisionRange;//bot left 
		collisionRangeFinal[1] = _boundSprite->getPosition() + _collisionRange; //top right

		math::Vector2 origin = vectorOrigin;
		math::Vector2 endpoint = vectorEndpoint;


		//swaping formula to positives
		float lowesty = std::min(std::min(collisionRangeFinal[0].y, vectorOrigin.y), vectorEndpoint.y);
		float lowestx = std::min(std::min(collisionRangeFinal[0].x, vectorOrigin.x), vectorEndpoint.x);

		if (lowestx < 0)
		{
			origin.x -= lowestx;
			endpoint.x -= lowestx;
			collisionRangeFinal[0].x -= lowestx;
			collisionRangeFinal[1].x -= lowestx;
		}

		if (lowesty < 0)
		{
			origin.y -= lowesty;
			endpoint.y -= lowesty;
			collisionRangeFinal[0].y -= lowesty;
			collisionRangeFinal[1].y -= lowesty;
		}

		//swaping end

		float closey;
		float closex;
		float m = (endpoint.y - origin.y) / (endpoint.x - origin.x);
		float x;
		float y;

		if (origin.y < collisionRangeFinal[0].y)
			closey = collisionRangeFinal[0].y;
		else if (origin.y > collisionRangeFinal[1].y)
			closey = collisionRangeFinal[1].y;
		else
			closey = 0;
			
			

		if (origin.x < collisionRangeFinal[0].x)
			closex = collisionRangeFinal[0].x;
		else if (origin.x > collisionRangeFinal[1].x)
			closex = collisionRangeFinal[1].x;
		else
			closex = 0;

		if (closex == 0)
		{
			x = origin.x + (1.0f / m) * (closey - origin.y);
			y = closey;
		}
		else
		{
			y = origin.y + m * (closex - origin.x);
			x = closex;

			if (!(y >= collisionRangeFinal[0].y && y <= collisionRangeFinal[1].y))
				return -1;
		}
		
		//vracanje u prvobitan predznak
		if (lowesty < 0)
		{
			y += lowesty;
		}

		if (lowestx < 0)
		{
			x += lowestx;
		}
		//

		if (!isHit(math::Vector2(x, y)))
			return -1;
		else
			return sqrtf(powf((origin.x - x), 2) + powf((origin.y - y), 2));

	}

	bool Hitbox::willBeHit(const Hitbox& other, const math::Vector2 nextMove) const
	{
		math::Vector2 collisionRangeFinal[2];
		collisionRangeFinal[0] = _boundSprite->getPosition() + nextMove - _collisionRange;
		collisionRangeFinal[1] = _boundSprite->getPosition() + nextMove + _collisionRange;

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