#include "Ammo.h"
namespace objects {
	Ammo::Ammo()
	{
	}

	Ammo::Ammo(const graphics::Sprite& sprite, float duration, unsigned int quantity)
		:Pickup(sprite, duration), _quantity(quantity)
	{
	}

	void Ammo::onPickup(Actor& actor)
	{
		if (actor.getWeapon() != nullptr)
		{
			Weapon* weapon = actor.getWeapon();
			unsigned int clipDifference = weapon->getClipMax() - weapon->getClipCurrent();
			if (_quantity >= clipDifference)
			{
				_quantity -= clipDifference;
				weapon->setClipCurrent(weapon->getClipMax());
			}
			else
			{
				weapon->setClipCurrent(weapon->getClipCurrent() + _quantity);
				_quantity = 0;
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
				else if (_quantity != 0)
				{
					weapon->setAmmoCurrent(weapon->getAmmoCurrent() + _quantity);
					destroy();
				}
			}
		}
	}

	void Ammo::destroy()
	{
		Pickup::destroy();
	}
}