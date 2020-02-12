#pragma once
#include "Pickup.h"
namespace inventory
{

	class Ammo : public Pickup
	{
		unsigned int _quantity;
		unsigned int _currentQuantity;
	public:
		Ammo();
		Ammo(graphics::Sprite* sprite, float duration, unsigned int quantity);
		void onPickup();
		void destroy();
	};
}