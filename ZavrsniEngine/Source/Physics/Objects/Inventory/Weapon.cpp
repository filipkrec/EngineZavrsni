#include "Weapon.h"
namespace objects
{
	Weapon::Weapon()
	{

	}

	Weapon::Weapon(const Weapon& other)
		: Sprite(other), _dmgMin(other._dmgMin), _dmgMax(other._dmgMax), _force(other._force), _range(other._range), _spread(other._spread), _ammoMax(other._ammoMax), _ammoCurrent(other._ammoMax), _weaponState(WeaponState::DROPED), _shotOriginOffset(other._shotOriginOffset)
		,_shotCooldownTime(other._shotCooldownTime),_reloadTime(other._reloadTime),_reloadTimer(engine::Timer(other._reloadTime)),_shotCooldownTimer(engine::Timer(other._shotCooldownTime))
	{
		_destroySprite = false;
	}

	Weapon::Weapon(const graphics::Sprite& sprite, unsigned int weight, unsigned int dmgMin, unsigned int dmgMax, float force, float range, unsigned int spread, unsigned int ammoMax, unsigned int clipMax, math::Vector2 shotOriginOffset)
		: Sprite(sprite), _dmgMin(dmgMin), _dmgMax(dmgMax), _force(force), _range(range), _spread(spread), _ammoMax(ammoMax), _ammoCurrent(ammoMax), _clipMax(clipMax), _clipCurrent(clipMax), _weaponState(WeaponState::DROPED), _shotOriginOffset(shotOriginOffset)
	{
		_destroySprite = false;
	}

	Weapon* Weapon::clone()
	{
		return new Weapon(*this);
	}

	void Weapon::reload(bool infinite)
	{
		if (_reloadTimer.elapsed() >= _reloadTime)
		{
			_reloadTimer.reset();
			if (!infinite)
			{
				unsigned int clipDifference = _clipMax - _clipCurrent;
				unsigned int load = _ammoCurrent >= clipDifference ? clipDifference : _ammoCurrent;
				_ammoCurrent -= load;
				_clipCurrent += load;
			}
			else
			{
				_clipCurrent = _clipMax;
			}
		}
	}

	void Weapon::shoot()
	{
		if (_clipCurrent > 0 && _reloadTimer.elapsed() >= _reloadTime && _shotCooldownTimer.elapsed() >= _shotCooldownTime)
		{
			_clipCurrent -= 1;
			float rotation = getRotation() + (_spread != 0 ? fmod((float)rand(), _spread) - _spread / 2 : 0);
			math::Vector2 destinationPoint = math::Matrix4::rotate2d(math::Vector2(_range, 0), _rotation); //vektor razlike od origina pucnjave do maksimalnog kraja
			_firedShots.push_back(getShotPosition() + destinationPoint);
			_shotCooldownTimer.reset();
		}
	}

	void Weapon::clearShots()
	{
		_firedShots.clear();
	}

	const math::Vector2 Weapon::getShotPosition() const
	{
		return getPosition() + math::Matrix4::rotate2d(_shotOriginOffset, _rotation);
	}
}