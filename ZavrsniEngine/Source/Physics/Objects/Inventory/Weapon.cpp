#include "Weapon.h"

namespace inventory {
	Weapon::Weapon()
	{

	}

	Weapon::Weapon(const Weapon& other)
		:GameObject(other._boundSprite, false),_dmgMin(other._dmgMin), _dmgMax(other._dmgMax), _range(other._range), _spread(other._spread), _ammoMax(other._ammoMax), _ammoCurrent(other._ammoMax), _weaponState(WeaponState::DROPED)
	{
	}

	Weapon::Weapon(graphics::Sprite * sprite, unsigned int weight, unsigned int dmgMin, unsigned int dmgMax, float range, float spread, unsigned int ammoMax, unsigned int clipMax)
		: GameObject(sprite, false),_dmgMin(dmgMin),_dmgMax(dmgMax),_range(range), _spread(spread),_ammoMax(ammoMax),_ammoCurrent(ammoMax),_clipMax(clipMax),_clipCurrent(clipMax), _weaponState(WeaponState::DROPED)
	{
	}

	Weapon* Weapon::clone()
	{
		return new Weapon(*this);
	}

	void Weapon::reload(bool infinite)
	{
		if (!infinite)
		{
			unsigned int clipDifference = _clipMax - _clipCurrent;
			unsigned int load = _ammoCurrent >= clipDifference ? clipDifference : _ammoCurrent;
			_ammoCurrent -= load;
		}
		_clipCurrent = _clipMax;
	}
	
	void Weapon::shoot()
	{

	}
}