#pragma once
#include "../GameObject.h"
namespace inventory
{
	enum WeaponState
	{
		CARRIED,
		DROPED
	};


	class Weapon:public physics::GameObject
	{
		unsigned int _dmgMin;
		unsigned int _dmgMax;
		float _range;
		float _spread;
		unsigned int _ammoMax;
		unsigned int _ammoCurrent;
		unsigned int _clipMax;
		unsigned int _clipCurrent;
	public:
		WeaponState _weaponState;

		Weapon();
		Weapon(const Weapon& other);
		Weapon(graphics::Sprite* sprite, unsigned int weight, unsigned int dmgMin, unsigned int dmgMax, float range, float spread, unsigned int ammoMax, unsigned int clipMax);

		void bind(const graphics::Sprite* sprite);
		void unbind();
		Weapon* clone();

		void reload(bool infinite = false);
		void shoot();
	};
}