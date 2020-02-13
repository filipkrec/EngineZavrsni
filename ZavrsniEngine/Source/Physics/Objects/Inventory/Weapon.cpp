#include "Weapon.h"
namespace objects
{
		Weapon::Weapon()
		{

		}

		Weapon::Weapon(const Weapon& other)
			: Pickup(other._boundSprite, 0), _dmgMin(other._dmgMin), _dmgMax(other._dmgMax), _force(other._force), _range(other._range), _spread(other._spread), _ammoMax(other._ammoMax), _ammoCurrent(other._ammoMax), _weaponState(WeaponState::DROPED), _shotOriginOffset(other._shotOriginOffset)
		{
		}

		Weapon::Weapon(graphics::Sprite* sprite, unsigned int weight, unsigned int dmgMin, unsigned int dmgMax, float force, float range, unsigned int spread, unsigned int ammoMax, unsigned int clipMax, math::Vector2 shotOriginOffset)
			: Pickup(sprite, 0), _dmgMin(dmgMin), _dmgMax(dmgMax), _force(force), _range(range), _spread(spread), _ammoMax(ammoMax), _ammoCurrent(ammoMax), _clipMax(clipMax), _clipCurrent(clipMax), _weaponState(WeaponState::DROPED), _shotOriginOffset(shotOriginOffset)
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
			if (_clipCurrent > 0)
			{
				_clipCurrent -= 1;
				float rotation = _boundSprite->getRotation() + (_spread != 0 ? rand() % _spread - _spread / 2 : 0);
				math::Vector2 destinationPoint = math::Vector2(math::Matrix4::rotation(rotation + _boundSprite->getRotation(), math::Vector3(0, 0, 1)) * math::Vector2(_range, 0)); //vektor razlike od origina pucnjave do maksimalnog kraja
				_firedShots.push_back(destinationPoint + _boundSprite->getPosition());
			}
		}

		void Weapon::destroy() {};
		void Weapon::onPickup(Actor& actor) {};
}