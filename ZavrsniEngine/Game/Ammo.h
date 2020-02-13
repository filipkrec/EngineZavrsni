#pragma once
#include "../Source/Engine.h"

namespace objects {
		class Ammo : public Pickup
		{
			unsigned int _quantity;
			unsigned int _currentQuantity;
		public:
			Ammo();
			Ammo(graphics::Sprite* sprite, float duration, unsigned int quantity);
			void onPickup(Actor& actor);
			void destroy();
		};
}