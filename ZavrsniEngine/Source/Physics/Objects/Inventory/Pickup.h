#pragma once
#include "../GameObject.h"
#include "../../../Util/Timer.h"
namespace objects {
	class Actor;

		class Pickup : public GameObject
		{
			engine::Timer _timer;
			float _duration;

			bool _destroyPickup;
		public:
			Pickup() : GameObject() {}
			Pickup(graphics::Sprite* sprite, float duration) : GameObject(sprite), _duration(duration), _timer(engine::Timer()),_destroyPickup(false) {};
			void move() override {};
			virtual void onPickup (Actor& actor) = 0;
			virtual void destroy() { _destroyPickup = true; };
			bool toDestroy() {return _destroyPickup;};
			virtual void processTime()
			{
				if (_duration > 0)
				{
					if (_timer.elapsed() > _duration)
					{
						destroy();
					}
				}
			}
		};
	}