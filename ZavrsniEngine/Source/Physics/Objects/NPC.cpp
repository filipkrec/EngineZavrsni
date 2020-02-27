#include "NPC.h"

namespace objects {
	std::vector<math::Vector2> NPC::directionsAll;

	NPC::NPC()
	{
	}

	NPC::NPC(GameObject gameObject, unsigned int health, float movementSpeed)
		:Actor(gameObject, health, movementSpeed, Actor::State::STATE_STILL)
	{
	}

	void NPC::process(const engine::Window& window)
	{
	}


	void NPC::moveInDirection()
	{
		if (_moveDirection != math::Vector2(0.0f, 0.0f))
		{
			calculateColission(math::Vector3(_moveDirection.x, _moveDirection.y, (_movementSpeed * MOVEMENT_SPEED_COEFFICIENT) * _weight));
		}
	}

	void NPC::lookAt()
	{
		if(_lookingAt != nullptr && !_lookingAt->toDestroySprite())
		rotateToPoint(_lookingAt->getPosition() - _boundSprite->getPosition()); //vektor udaljenosti
	}
	 
	void NPC::init()
	{
		Actor::init();
		if (directionsAll.empty())
		{
			math::Vector2 right = math::Vector2::calculateUnitVector(math::Vector2(1, 0));
			directionsAll.push_back(right);
			math::Vector2 up = math::Vector2::calculateUnitVector(math::Vector2(0, 1));
			directionsAll.push_back(up);
			math::Vector2 left = math::Vector2::calculateUnitVector(math::Vector2(-1, 0));
			directionsAll.push_back(left);
			math::Vector2 down = math::Vector2::calculateUnitVector(math::Vector2(0, -1));
			directionsAll.push_back(down);
			math::Vector2 botRight = math::Vector2::calculateUnitVector(math::Vector2(1, -1));
			directionsAll.push_back(botRight);
			math::Vector2 topRight = math::Vector2::calculateUnitVector(math::Vector2(1, 1));
			directionsAll.push_back(topRight);
			math::Vector2 botLeft = math::Vector2::calculateUnitVector(math::Vector2(-1, -1));
			directionsAll.push_back(botLeft);
			math::Vector2 topleft = math::Vector2::calculateUnitVector(math::Vector2(-1, 1));
			directionsAll.push_back(topleft);
		}
	}

	void NPC::setMoveDirection(const math::Vector2& direction)
	{
		_moveDirection = direction;
	}

	void NPC::setLookingAt(graphics::Sprite* sprite)
	{
		_lookingAt = sprite;
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