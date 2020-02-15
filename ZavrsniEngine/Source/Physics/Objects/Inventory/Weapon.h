#pragma once
#include "../../../Graphics/Sprite.h"
namespace objects {
		enum WeaponState
		{
			CARRIED,
			DROPED
		};


		class Weapon :public graphics::Sprite
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

			math::Vector2 _shotOriginOffset; // offset from center of carrying sprite
		public:
			WeaponState _weaponState;
			std::vector<math::Vector2> _firedShots; // destination points 

			Weapon();
			Weapon(const Weapon& other);
			Weapon(graphics::Sprite& sprite, unsigned int weight, unsigned int dmgMin, unsigned int dmgMax, float force, float range, unsigned int spread, unsigned int ammoMax, unsigned int clipMax, math::Vector2 shotOriginOffset);

			inline const unsigned int getAmmoMax() const { return _ammoMax; }
			inline const unsigned int getAmmoCurrent() const { return _ammoCurrent; }
			inline const unsigned int getDmgMax() const { return _dmgMax; }
			inline const unsigned int getDmgMin() const { return _dmgMin; }
			inline const unsigned int getClipMax() const { return _clipMax; }
			inline const unsigned int getClipCurrent() const { return _clipCurrent; }
			inline const float getForce() const { return _force; }


			inline void setAmmoMax(unsigned int value) {  _ammoMax = value; }
			inline void setAmmoCurrent(unsigned int value) {  _ammoCurrent = value; }
			inline void setDmgMax(unsigned int value) {  _dmgMax = value; }
			inline void setDmgMin(unsigned int value) {  _dmgMin = value; }
			inline void setClipMax(unsigned int value) {  _clipMax = value; }
			inline void setClipCurrent(unsigned int value) {  _clipCurrent = value; }

			Weapon* clone();

			void reload(bool infinite = false);
			void shoot();
			void clearShots();
		};
}