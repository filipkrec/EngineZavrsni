#pragma once
#include "Engine.h"

class Rifle: public objects::Weapon
{
public:
	Rifle();
	Rifle(const math::Vector2& spriteSize,const math::Vector2& modelOffset, const math::Vector2& shotOffset, const graphics::Texture* texture);
	Weapon* clone() override;
	void onShoot() override;
	void onReload() override;
};