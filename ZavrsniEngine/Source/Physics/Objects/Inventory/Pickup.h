#pragma once
#include "../GameObject.h"
#include "../Actor.h"
#include "../../../Util/Timer.h"
namespace objects {

		class Pickup : public GameObject
		{
			engine::Timer _timer;
			float _duration;
		public:
			Pickup() : GameObject() {}
			Pickup(graphics::Sprite* sprite, float duration) : GameObject(sprite), _duration(0.0f), _timer(engine::Timer()) {};
			void move() override {};
			virtual void onPickup(Actor& actor) = 0;
			virtual void destroy() = 0;
		};
	}