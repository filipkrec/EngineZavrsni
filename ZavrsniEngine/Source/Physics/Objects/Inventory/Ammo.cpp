#pragma once
#include "Ammo.h"
namespace objects {
		Ammo::Ammo()
		{
		}

		Ammo::Ammo(graphics::Sprite* sprite, float duration, unsigned int quantity)
			:Pickup(sprite, duration), _quantity(quantity)
		{
		}

		void Ammo::onPickup(Actor& actor)
		{
			actor._weapon
		}

		void Ammo::destroy()
		{
			delete this->getSprite();
			delete this;
		}
}