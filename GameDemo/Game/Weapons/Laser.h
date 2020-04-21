#pragma once
#include "Engine.h"

class Laser: public objects::Weapon
{
public:
	Laser();
	Weapon* clone() override;
	void onShoot() override;
};