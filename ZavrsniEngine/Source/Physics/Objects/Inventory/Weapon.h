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
		unsigned int _dmgMin; //random min dmg
		unsigned int _dmgMax; //random max dmg
		float _force; //max force output of shot, reduced over distance %range = %force
		float _range; //max range 
		unsigned int _spread; // angle offset * 2
		unsigned int _ammoMax; //holding max ammo
		unsigned int _ammoCurrent; //holding current ammo
		unsigned int _clipMax; //clip max ammo
		unsigned int _clipCurrent; //clip current ammo

		std::vector<math::Vector2> _firedShots; // destination points 
		math::Vector2 _shotOriginOffset; // offset from center of carrying sprite
	public:
		WeaponState _weaponState;

		Weapon();
		Weapon(const Weapon& other);
		Weapon(graphics::Sprite* sprite, unsigned int weight, unsigned int dmgMin, unsigned int dmgMax, float force, float range, unsigned int spread, unsigned int ammoMax, unsigned int clipMax,math::Vector2 shotOriginOffset);
		void move() override;

		Weapon* clone();

		void reload(bool infinite = false);
		void shoot();
	};
}