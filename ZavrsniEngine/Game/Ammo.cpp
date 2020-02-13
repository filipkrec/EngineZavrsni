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
			Weapon* weapon = actor.getWeapon();
			unsigned int clipDifference =  weapon->getClipMax() - weapon->getClipCurrent();
			if (_quantity >= clipDifference)
			{
				_quantity -= clipDifference;
				weapon->setClipCurrent(weapon->getClipMax());
			}
			else
			{
				weapon->setClipCurrent(weapon->getClipCurrent() + clipDifference);
				destroy();
			}

			if (_quantity > 0)
			{
				unsigned int ammoDifference = weapon->getAmmoMax() - weapon->getAmmoCurrent();
				if (_quantity >= ammoDifference)
				{
					_quantity -= ammoDifference;
					weapon->setAmmoCurrent(weapon->getAmmoMax());
				}
				else
				{
					weapon->setAmmoCurrent(weapon->getAmmoCurrent() + ammoDifference);
					destroy();
				}
			}
		}

		void Ammo::destroy()
		{
			delete this->getSprite();
			delete this;
		}
}