#pragma once
#include "Actor.h"

namespace physics
{
	class NPC :Actor
	{
		bool _friendly;
	public:
		NPC();
		void animate();
		void processState();
		void processAI();
		void process();
	};
}