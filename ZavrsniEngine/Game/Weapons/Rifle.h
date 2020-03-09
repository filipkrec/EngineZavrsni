#pragma once
#include "../../Source/Engine.h"

class Rifle: public objects::Weapon
{
	static Rifle* instance;
	Rifle();
public:
	static Rifle* getInstance();
	void onShot(objects::GameObject* target) override;
};