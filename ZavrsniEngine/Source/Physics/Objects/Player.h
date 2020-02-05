#pragma once
#include "Actor.h"

namespace physics
{
	class Player :Actor
	{
	public:
		Player();
		void animate();
		void processState();
		void processInput();
	};
}