#if 1
#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include "Source/Graphics/Layer.h"
#include "Source/Util/Timer.h"
#include "Source/Graphics/Group.h"
#include "Source/Physics/Objects/GameObject.h"
#include "Source/Audio/AudioManager.h"
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
	Texture* texturePlanet = new Texture("Assets/test2.png");
	Texture* textureSpace = new Texture("Assets/test3.png", true);
	Texture* texturePlayer = new Texture("Assets/playertest.png");

	Sprite* space = new Sprite(-16.0f, -9.0f, 32.0f, 18.0f, textureSpace);

	GameObject playerGO(Sprite(0.0f, 0.0f, 6.0f, 2.0f, texturePlayer, 2), 100);
	GameObject planetGO(Sprite(-4.0f, -4.0f, 2.0f, 2.0f, texturePlanet, 1), 200);

	AudioManager::init();
	AudioManager::add(new Audio("Test", "Assets/test.wav"));
	AudioManager::get("Test")->play();

	layer->add(&planetGO._sprite);
	layer->add(&playerGO._sprite);
	layer->add(space);

	Timer* timer = new Timer();
	Timer* timerTick = new Timer();
	Timer* timerForce = new Timer();
	Timer* timerForce2 = new Timer();
	int fps = 0;

	double x, y;
	while (!display->closed())
	{
		display->clear();
		fps++;
		display->getMousePosition(x, y);
		x = (x / 800) * 32 - 16;
		y = - ((y / 600) * 18 - 9);
		audio::AudioManager::update();

		if (timerTick->elapsed() >= 1.0f / 60.0f)
		{
			timerTick->reset();
			playerGO.move();
			planetGO.move();
		}

		if (timer->elapsed() >= 1.0f)
		{
			std::cout << fps << std::endl;
			timer->reset();
			fps = 0;
		}

		if (timerForce->elapsed() >= 3.0f)
		{
			//playerGO.calculateColission(math::Vector3(1.0f, 1.0f, 150));
			planetGO.calculateColission(math::Vector3(1.0f, 1.0f, 100));
			timerForce->reset();
		}


		if (timerForce2->elapsed() >= 4.0f)
		{
			//playerGO.calculateColission(math::Vector3(-1.0f, 1.0f, 150));
			timerForce2->reset();
			planetGO.calculateColission(math::Vector3(1.0f, -1.0f, 200));
		}

		layer->render();
		display->update();
	}
}
#endif