#pragma once
#include "Actor.h"

namespace objects
{
	class NPC :public Actor
	{
		bool _friendly;
	public:
		NPC();
		virtual void animate();
		virtual void processState();
		virtual void processAI();
		void moveInDirection(const math::Vector2 direction);
		void lookAt(const graphics::Sprite* sprite);
		void process();
	};
}