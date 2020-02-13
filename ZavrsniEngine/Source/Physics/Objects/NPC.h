#pragma once
#include "Actor.h"

namespace objects
{
	class NPC :public Actor
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