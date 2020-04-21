#include "Laser.h"

Laser::Laser()
	:Weapon()
{
	_color = 0x00000000;
	_reloadTime = 0.0f;
	_reloadTimer = engine::Timer(_reloadTime);
	_shotCooldownTime = 0.4f;
	_shotCooldownTimer = engine::Timer(_shotCooldownTime);
	_dmgMin = 5.0f;
	_dmgMax = 10.0f;
	_force = 100.0f;
	_range = 8.0f;
	_spread = 20.0f; // angle offset * 2
	_ammoMax = 999.0f,
	_ammoCurrent = 999.0f; //holding current ammo
	_clipMax = 999.0f; //clip max ammo
	_clipCurrent = 999.0f; //clip current ammo
	_size = math::Vector2(0.0f, 0.0f);
	_position = math::Vector2(0.0f, 0.0f);
	_shotOriginOffset = math::Vector2(0.0f,0.0f);
	_texture = nullptr;
}

objects::Weapon* Laser::clone()
{
	return new Laser();
}

void Laser::onShoot()
{
	if (audio::AudioManager::get("Laser") != nullptr)
		audio::AudioManager::get("Laser")->play();
}