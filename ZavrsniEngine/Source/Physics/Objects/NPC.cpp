#include "NPC.h"

namespace objects {
	void NPC::process()
	{
		
	}


	void NPC::moveInDirection(const math::Vector2 direction)
	{
		calculateColission(math::Vector3(direction.x, direction.y, (_movementSpeed * MOVEMENT_SPEED_COEFFICIENT) * _weight));
	}

	void NPC::lookAt(const graphics::Sprite* sprite)
	{
		rotateToPoint(sprite->getPosition() - _boundSprite->getPosition()); //vektor udaljenosti
	}
}

/*
int main() {
	for (int i = 0; i & lt; sizeof(points) / sizeof(point); ++i)
	{
		curval = points[i].x == 0 ? 0 : points[i].x / points[i].y;
		if (highval& lt; = curval)
		{
			highval = curval;
			highest = points[i];
		}

		if (lowval& gt; = curval)
		{
			lowval = curval;
			lowest = points[i];
		}
	}
}
*/