#if 1
#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Layer.h"
#include "Source/Util/Timer.h"
#include "Source/Graphics/Group.h"
#include "Source/Managers/AudioManager.h"
#include "Source/Managers/TextureManager.h"
#include "Source/Managers/LevelAssetManager.h"
#include "Source/Graphics/Camera.h"
#include "Source/Physics/Objects/Player.h"
#include "Game/Pickups/Ammo.h"
#include "Source/Physics/Objects/Inventory/WeaponObject.h"
#include <iostream>
#define FRICTION 1.0f
#define PROCESSING_INTERVAL 60

int main()
{
	using namespace engine;
	using namespace graphics;
	using namespace objects;
	using namespace math;
	using namespace audio;
	Window* display = new Window("Display", 800, 600);
	Layer* layer = new Layer();
	TextureManager::add(new Texture("Assets/test2.png"), "Planet");
	TextureManager::add(new Texture("Assets/test3.png"), "Space");
	TextureManager::add(new Texture("Assets/test.png"), "!!");
	TextureManager::add(new Texture("Assets/playertest.png"), "Player");
	TextureManager::add(new Texture("Assets/ammo.png"), "Ammo");
	TextureManager::add(new Texture("Assets/rifle.png"), "Rifle");
	TextureManager::add(new Texture("Assets/riflePickup.png"), "RiflePickup");

	Font* font = new Font("Assets/arial.ttf", 20);
	font->setScale(16, 9);

	lam::LevelAssetManager::init(new Player(GameObject(new Sprite(0.0f, 0.0f, 2.0f, 2.0f, TextureManager::get("Player"),2), 100), 100, 150), layer);
	lam::LevelAssetManager::add(new Label("100/100", -10.0f, 6.0f, 0xff00ff00, font, 16), "AmmoLabel");
	Weapon* weapon = new Weapon(Sprite(0.0f, 0.0f, 1.0f, 0.5f, TextureManager::get("Rifle"), 2), 0, 10, 10, 200.0f, 20.0f, 1, 100, 50, math::Vector2(1.0f, 0.0f));
	lam::LevelAssetManager::add((Pickup*)(new Ammo(new Sprite(-4.0f, -4.0f, 1.0f, 1.0f, TextureManager::get("Ammo"), 1), 5, 20)),"Ammo");
	lam::LevelAssetManager::add((Pickup*)(new WeaponObject(new Sprite(0.0f, -4.0f, 1.0f, 1.0f, TextureManager::get("RiflePickup"), 1), weapon)),"WeaponObject");
	lam::LevelAssetManager::add(new Sprite(0.0f, 0.0f, 32.0f, 18.0f, TextureManager::get("Space"),0), "Space");
	//lam::LevelAssetManager::add(new Sprite(0.0f, 0.0f, 6.0f, 2.0f, TextureManager::get("Player"), 1), "Player");

	//lam::LevelAssetManager::add(new GameObject(Sprite(0.0f, 0.0f, 6.0f, 2.0f, TextureManager::get("Player"), 1), 100), "Player");
	lam::LevelAssetManager::add(new GameObject(&Sprite(5.0f, 5.0f, 2.0f, 2.0f, TextureManager::get("Planet"), 2), 300), "Planet");
	lam::LevelAssetManager::add(new GameObject(&Sprite(-3.0f, -3.0f, 2.0f, 2.0f, TextureManager::get("Planet"), 2), 300), "Planet2");

	lam::LevelAssetManager::getPlayer()->setSight(45.0f,10.0f);
	lam::LevelAssetManager::getPlayer()->setOnSightFunction([](GameObject* x)
		{
			std::cout << x->getSpritePosition().x << std::endl;
		}
	);

	lam::LevelAssetManager::addToLayer(layer);
	Timer* timer = new Timer();
	Timer* timerTick = new Timer();
	int fps = 0;

	float x, y;
	while (!display->closed())
	{
		display->clear();
		lam::LevelAssetManager::processBegin(*display);
		fps++;
		display->getMousePosition(x, y);
		if (timerTick->elapsed() >= 1.0f / 60.0f)
		{
			Player* player = lam::LevelAssetManager::getPlayer();
			Hitbox* hitbox = lam::LevelAssetManager::getPlayer();
			GameObject* planet = lam::LevelAssetManager::getGameObject("Planet");
			Label* label = lam::LevelAssetManager::getLabel("AmmoLabel");
			if (player->getWeapon() != nullptr)
			{
				unsigned int maxclip = player->getWeapon()->getClipMax();
				unsigned int clip = player->getWeapon()->getClipCurrent();
				label->setText(std::to_string(clip) + " / " + std::to_string(maxclip));
			}
			/*
			if (planet->isHit(*hitbox))
				planet->getSprite()->swapTexture(TextureManager::get("!!"));
			else
				planet->getSprite()->swapTexture(TextureManager::get("Planet"));
			*/
			//Camera::getInstance()->move(Vector2(-0.01f, 0.0f));
			timerTick->reset();
		}


		if (timer->elapsed() >= 1.0f)
		{
			//std::cout << fps << std::endl;

			timer->reset();
			fps = 0;
		}

		lam::LevelAssetManager::processEnd(*display);
		layer->render();
		display->update();
	}
}
#endif