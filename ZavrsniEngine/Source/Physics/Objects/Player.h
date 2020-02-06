#pragma once
#include "Actor.h"

namespace physics
{
	class Player :public Actor
	{
	public:
		Player();
		void animate();
		void processState();
		void processInput();
	};
}