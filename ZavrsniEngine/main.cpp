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
#include <iostream>
#define AT_JOB 0

int main()
{
	using namespace engine;
	using namespace graphics;
	using namespace physics;
	using namespace math;
	using namespace audio;
	Window* display = new Window("Display", 800, 600);
	Layer* layer = new Layer();
	TextureManager::add(new Texture("Assets/test2.png"), "Planet");
	TextureManager::add(new Texture("Assets/test3.png"), "Space");
	TextureManager::add(new Texture("Assets/test.png"), "!!");
	TextureManager::add(new Texture("Assets/playertest.png"), "Player");

	lam::LevelAssetManager::init(new Player(GameObject(new Sprite(0.0f, 0.0f, 2.0f, 2.0f, TextureManager::get("Player"),2), 100), 100, 300));
	lam::LevelAssetManager::add(new Sprite(0.0f, 0.0f, 32.0f, 18.0f, TextureManager::get("Space"),0), "Space");
	//lam::LevelAssetManager::add(new Sprite(0.0f, 0.0f, 6.0f, 2.0f, TextureManager::get("Player"), 1), "Player");

	//lam::LevelAssetManager::add(new GameObject(Sprite(0.0f, 0.0f, 6.0f, 2.0f, TextureManager::get("Player"), 1), 100), "Player");
	lam::LevelAssetManager::add(new GameObject(&Sprite(5.0f, 5.0f, 2.0f, 2.0f, TextureManager::get("Planet"), 2), 100,Shape::SQUARE,1.5f), "Planet");

	lam::LevelAssetManager::addToLayer(layer);
	Timer* timer = new Timer();
	Timer* timerTick = new Timer();
	int fps = 0;

	float x, y;
	while (!display->closed())
	{
		display->clear();
		fps++;

		lam::LevelAssetManager::process(*display);
		display->getMousePosition(x, y);
		if (timerTick->elapsed() >= 1.0f / 60.0f)
		{
			Player* player = lam::LevelAssetManager::getPlayer();
			Hitbox* hitbox = lam::LevelAssetManager::getPlayer();
			GameObject* planet = lam::LevelAssetManager::getGameObject("Planet");
			
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
			std::cout << fps << std::endl;

			timer->reset();
			fps = 0;
		}

		layer->render();
		display->update();
	}
}
#endif