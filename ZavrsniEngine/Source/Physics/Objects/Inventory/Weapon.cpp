#include "Weapon.h"
namespace objects
{
	unsigned int Weapon::_idCurrent = 0;

	Weapon::Weapon()
		:_id(_idCurrent)
	{
		_idCurrent++;
	}

	Weapon::Weapon(const graphics::Sprite& sprite)
		: Sprite(sprite), _id(_idCurrent), _shotOriginOffset(math::Vector2(0,0))
	{
		_idCurrent++;
		_destroySprite = false;
	}

	Weapon::Weapon(const Weapon& other)
		: Sprite(other), _id(_idCurrent), _dmgMin(other._dmgMin), _dmgMax(other._dmgMax), _force(other._force), _range(other._range), _spread(other._spread), _ammoMax(other._ammoMax), _ammoCurrent(other._ammoMax),_clipCurrent(other._clipMax), _clipMax(other._clipMax), _shotOriginOffset(other._shotOriginOffset)
		,_shotCooldownTime(other._shotCooldownTime),_reloadTime(other._reloadTime),_reloadTimer(engine::Timer(other._reloadTime)),_shotCooldownTimer(engine::Timer(other._shotCooldownTime))
	{
		_idCurrent++;
		_destroySprite = false;
	}

	Weapon::Weapon(const graphics::Sprite& sprite, unsigned int weight, unsigned int dmgMin, unsigned int dmgMax, float force, float range, unsigned int spread, unsigned int ammoMax, unsigned int clipMax, math::Vector2 shotOriginOffset)
		: Sprite(sprite), _id(_idCurrent), _dmgMin(dmgMin), _dmgMax(dmgMax), _force(force), _range(range), _spread(spread), _ammoMax(ammoMax), _ammoCurrent(ammoMax), _clipMax(clipMax), _clipCurrent(clipMax), _shotOriginOffset(shotOriginOffset)
	{
		_idCurrent++;
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
			onReload();
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
			_reloadTimer.reset();
		}
	}

	void Weapon::shoot()
	{
		if (_clipCurrent > 0 && _reloadTimer.elapsed() >= _reloadTime && _shotCooldownTimer.elapsed() >= _shotCooldownTime)
		{
			onShoot();
			_clipCurrent -= 1;
			float rotation = getRotation() + (_spread != 0 ? fmod((float)rand(), _spread) - _spread / 2 : 0);
			math::Vector2 destinationPoint = math::Matrix4::rotate2d(math::Vector2(_range, 0), _rotation); //vektor razlike od origina pucnjave do maksimalnog kraja
			_firedShots.push_back(getShotPosition() + destinationPoint);
			_shotCooldownTimer.reset();
		}
	}


	void onShot(GameObject* target){}

	void Weapon::onShoot() {}
	void Weapon::onReload() {}
	void Weapon::clearShots()
	{
		_firedShots.clear();
	}

	const math::Vector2 Weapon::getShotPosition() const
	{
		return getPosition() + math::Matrix4::rotate2d(_shotOriginOffset, _rotation);
	}
}