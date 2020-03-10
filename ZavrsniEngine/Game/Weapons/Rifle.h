#pragma once
#include "../../Source/Engine.h"

class Rifle: public objects::Weapon
{
public:
	Rifle();
	Rifle(const math::Vector2& spriteSize,const math::Vector2& modelOffset, const math::Vector2& shotOffset, const graphics::Texture* texture);
	Rifle* clone();
	void onShot(objects::GameObject* target) const override;
};