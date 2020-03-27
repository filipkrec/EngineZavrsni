#pragma once
#include "../../Source/Engine.h"

class Laser: public objects::Weapon
{
public:
	Laser();
	Weapon* clone() override;
	void onShoot() override;
};