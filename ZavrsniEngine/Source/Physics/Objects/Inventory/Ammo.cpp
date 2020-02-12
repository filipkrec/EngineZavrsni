#pragma once
#include "Ammo.h"
namespace inventory
{
	Ammo::Ammo()
	{
	}

	Ammo::Ammo(graphics::Sprite* sprite, float duration, unsigned int quantity)
		:Pickup(sprite, duration),_quantity(quantity)
	{
	}

	void Ammo::onPickup()
	{
	}

	void Ammo::destroy()
	{
		delete this->getSprite();
		delete this;
	}
	
}