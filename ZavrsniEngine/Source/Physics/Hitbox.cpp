#include "Hitbox.h"
#include <algorithm>

namespace objects {
	Hitbox::Hitbox()
	{
	}


	Hitbox::~Hitbox()
	{
	}


	Hitbox::Hitbox(const math::Vector2 location, const math::Vector2 collisionRange)
		:_shape(Shape::SQUARE),_collisionRange(collisionRange),_location(location),_spriteless(true)
	{

	}

	Hitbox::Hitbox(const Hitbox& other)
		:_shape(other._shape),_collisionRange(other._collisionRange),_boundSprite(other._boundSprite), _spriteless(other._spriteless)
	{
		_boundSprite->DoNotDestroySprite();
	}

	Hitbox::Hitbox(graphics::Sprite* sprite)
		:_boundSprite(sprite),_shape(Shape::SQUARE), _spriteless(false)
	{
		math::Vector2 center = math::Vector2(0.0f, 0.0f);
		_collisionRange = math::Vector2(center.x + sprite->getSize().x/2, center.y + sprite->getSize().y / 2);
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

	Hitbox::Hitbox(graphics::Sprite* sprite, Shape shape, const float width, const float height) :
		_boundSprite(sprite), _shape(shape), _spriteless(false)
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
		:_boundSprite(sprite),_shape(shape), _collisionRange(collisionRange), _spriteless(false)
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

		math::Vector2 E = other.getSpritePosition() - other.getCollisionRange() + nextMove;
		math::Vector2 F = other.getSpritePosition() + math::Vector2(other.getCollisionRange().x, -other.getCollisionRange().y) + nextMove;
		math::Vector2 G = other.getSpritePosition() + other.getCollisionRange() + nextMove;
		math::Vector2 H = other.getSpritePosition() + math::Vector2(-other.getCollisionRange().x, other.getCollisionRange().y) + nextMove;

		math::Vector2 dots[4]{ getSpritePosition() - getCollisionRange(),
		getSpritePosition() + math::Vector2(getCollisionRange().x, -getCollisionRange().y),
		getSpritePosition() + getCollisionRange(),
		getSpritePosition() + math::Vector2(-getCollisionRange().x, getCollisionRange().y)};

		math::Vector2 edgesTop[2];
		math::Vector2 edgesBottom[2];
		math::Vector2 furthestEdges[2];
		bool inLine = false;

		if (nextMove.x < 0 && nextMove.y < 0) //ljevo dolje 
		{
			if (getLineIntersection(other.getSpritePosition(), A + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;

			edgesTop[0] = H;
			edgesTop[1] = D;
			edgesBottom[0] = F;
			edgesBottom[1] = B;
			furthestEdges[0] = E;
			furthestEdges[1] = C;
		}
		else if (nextMove.x == 0 && nextMove.y < 0) //dolje 
		{
			if (getLineIntersection(other.getSpritePosition(), other.getSpritePosition()
				- math::Vector2(0,other.getCollisionRange().y + 0.01) + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;
			inLine = true;
			furthestEdges[0] = E;
			furthestEdges[1] = C;
		}
		else if (nextMove.x > 0 && nextMove.y < 0) //desno dolje
		{
			if (getLineIntersection(other.getSpritePosition(), B + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;
			edgesTop[0] = C;
			edgesTop[1] = G;
			edgesBottom[0] = A;
			edgesBottom[1] = E;
			furthestEdges[0] = math::Vector2(D.x,F.y);
			furthestEdges[1] = math::Vector2(F.x, D.y);
		}
		else if (nextMove.x > 0 && nextMove.y == 0) //desno
		{
			if (getLineIntersection(other.getSpritePosition(), other.getSpritePosition()
				+ math::Vector2(other.getCollisionRange().x + 0.01,0) + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;
			inLine = true;
			furthestEdges[0] = A;
			furthestEdges[1] = G;
		}
		else if (nextMove.x > 0 && nextMove.y > 0) //desno gore
		{
			if (getLineIntersection(other.getSpritePosition(), C + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;
			edgesTop[0] = D;
			edgesTop[1] = H;
			edgesBottom[0] = B;
			edgesBottom[1] = F;
			furthestEdges[0] = A;
			furthestEdges[1] = G;
		}
		else if (nextMove.x == 0 && nextMove.y > 0) //gore
		{
			if (getLineIntersection(other.getSpritePosition(), other.getSpritePosition()
				+ math::Vector2(0,other.getCollisionRange().y + 0.01) + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;
			inLine = true;
			furthestEdges[0] = A;
			furthestEdges[1] = G;
		}
		else if (nextMove.x < 0 && nextMove.y > 0) //ljevo gore
		{
			if (getLineIntersection(other.getSpritePosition(), D + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;
			edgesTop[0] = D;
			edgesTop[1] = H;
			edgesBottom[0] = B;
			edgesBottom[1] = F;
			furthestEdges[0] = math::Vector2(H.x, B.y);
			furthestEdges[1] = math::Vector2(B.x, H.y);
		}
		else if (nextMove.x < 0 && nextMove.y == 0) //ljevo
		{
			if (getLineIntersection(other.getSpritePosition(), other.getSpritePosition()
				- math::Vector2(other.getCollisionRange().x + 0.01,0) + nextMove) != math::Vector4(0, 0, 0, 0))
				return true;
			inLine = true;
			furthestEdges[0] = E;
			furthestEdges[1] = C;
		}

		//provjera jeli hitbox van omedenog polja
		bool isInSquare = false;
		for (int i = 0; i < 4; ++i)
		{
			if (dots[i] >= furthestEdges[0] && dots[i] <= furthestEdges[1])
			{
				isInSquare = true;
				break;
			}
		}

		if (isInSquare == false)
			return false;
		else if (inLine == true)
			return true;

		//ako se nalazi unutar kvadrata i omeden je sa dva paralelna pravca od vrhova micajuceg objekta
		float slope = (edgesTop[1].y - edgesTop[0].y) / (edgesTop[1].x - edgesTop[0].x);
		// b = y - m * x
		float offsetTop = edgesTop[0].y - slope * edgesTop[0].x;
		float offsetBot = edgesBottom[0].y - slope * edgesBottom[0].x;


		for (int i = 0; i < 4; ++i)
		{
			float offsetDot = dots[i].y - slope * dots[i].x;
			if (offsetDot <= offsetTop && offsetDot >= offsetBot)
				return true;
		}

	}


	math::Vector4 Hitbox::getLineIntersection(const math::Vector2& vectorOrigin, const math::Vector2& vectorEndpoint) const
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

		int Hitbox::computeCode(const math::Vector2& vector) const
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