#pragma once
#include "../GameObject.h"
#include "../Actor.h"
#include "../../../Util/Timer.h"
namespace inventory
{

	class Pickup :public physics::GameObject
	{
		engine::Timer _timer;
		float _duration;
	public:		
		Pickup() : GameObject() {}
		Pickup(graphics::Sprite* sprite, float duration) : GameObject(sprite, false), _duration(0.0f),_timer(engine::Timer()) {};
		virtual void onPickup(physics::Actor& actor) = 0;
		virtual void destroy() = 0;
	};
}