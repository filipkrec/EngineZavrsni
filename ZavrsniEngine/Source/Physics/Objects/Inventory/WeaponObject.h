#pragma once
#include "Pickup.h"
#include "Weapon.h"
#include "../Actor.h"

namespace objects {
	class WeaponObject : public Pickup
	{
		Weapon* _weapon;
		void onPickup(Actor& actor) override
		{
			actor.setWeapon(_weapon);
			_weapon = nullptr;
		}
	};
}