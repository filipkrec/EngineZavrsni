#include "Rifle.h"

Rifle* Rifle::instance = 0;

Rifle* Rifle::getInstance()
{
	if (instance == 0)
	{
		instance = new Rifle();
	}

	return instance;
}

Rifle::Rifle()
	:Weapon(Sprite(0.0f, 0.0f, 1.0f, 0.5f, graphics::TextureManager::get("Rifle"), 2), 0, 10, 10, 200.0f, 20.0f, 1, 100, 50, math::Vector2(1.0f, 0.0f))
{
}


void Rifle::onShot(objects::GameObject* target)
{

}