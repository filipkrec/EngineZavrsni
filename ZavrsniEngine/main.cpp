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
	Layer* layer = new Layer(Matrix4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
	TextureManager::add(new Texture("Assets/test2.png"), "Planet");
	TextureManager::add(new Texture("Assets/test3.png"), "Space");
	TextureManager::add(new Texture("Assets/playertest.png"), "Player");

	lam::LevelAssetManager::init(new Player(GameObject(Sprite(0.0f, 0.0f, 6.0f, 2.0f, TextureManager::get("Player"), 2), 100), 100, 100));
	lam::LevelAssetManager::add(new Sprite(-16.0f, -9.0f, 32.0f, 18.0f, TextureManager::get("Space")), "Space");
	lam::LevelAssetManager::add(new GameObject(Sprite(-4.0f, -4.0f, 2.0f, 2.0f, TextureManager::get("Planet"), 1), 200), "Planet");

	lam::LevelAssetManager::addToLayer(layer);

	Timer* timer = new Timer();
	Timer* timerTick = new Timer();
	int fps = 0;

	float x, y;
	while (!display->closed())
	{
		display->clear();
		fps++;
		display->getMousePosition(x, y);
		x = (x / 800) * 32 - 16;
		y = - ((y / 600) * 18 - 9);

		lam::LevelAssetManager::process(*display);

		if (timerTick->elapsed() >= 1.0f / 60.0f)
		{
			timerTick->reset();
		}

		if (timer->elapsed() >= 1.0f)
		{
			std::cout << fps << std::endl;
			//lam::LevelAssetManager::getPlayer()->calculateColission(math::Vector3(1, 1, 500));
			timer->reset();
			fps = 0;
		}

		layer->render();
		display->update();
	}
}
#endif