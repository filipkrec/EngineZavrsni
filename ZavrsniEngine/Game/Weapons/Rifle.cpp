#include "Rifle.h"

Rifle::Rifle() //float zindex
	:Weapon(Sprite(0.0f, 0.0f, 0.0f, 0.0f, nullptr, 2), 0, 0, 0, 0, 0, 0, 0, 0, math::Vector2(0.0f, 0.0f))
{
	_reloadTime = 1.5f;
	_reloadTimer = engine::Timer(_reloadTime);
	_shotCooldownTime = 0.4f;
	_shotCooldownTimer = engine::Timer(_shotCooldownTime);
	_dmgMin = 15.0f;
	_dmgMax = 20.0f; 
	_force = 200.0f;
	_range = 20.0f; 
	_spread = 6.0f; // angle offset * 2
	_ammoMax = 40.0f,
	_ammoCurrent = 40.0f; //holding current ammo
	_clipMax = 5.0f; //clip max ammo
	_clipCurrent = 5.0f; //clip current ammo
}


Rifle::Rifle(const math::Vector2& spriteSize, const math::Vector2& modelOffset, const math::Vector2& shotOffset, const graphics::Texture* texture)
	:Rifle()
{
	_size = spriteSize;
	_position = modelOffset;
	_shotOriginOffset = shotOffset;
	_texture = texture;
}

Rifle* Rifle::clone()
{
	return new Rifle(_size, _position, _shotOriginOffset, _texture);
}

void Rifle::onShot(objects::GameObject* target) const
{

}