#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include "Source/Graphics/Layer.h"
#include "Source/Util/Timer.h"
#include "Source/Graphics/Group.h"
#include <iostream>
#define AT_JOB 0


#if 1
int main()
{
	using namespace engine;
	using namespace graphics;
	Window* display = new Window("Display", 1280, 920);
	Layer* layer = new Layer(math::Matrix4::orthographic(-16.0f,16.0f,-9.0f,9.0f,-1.0f,1.0f));
	Texture* texturePlanet = new Texture("test2.png");
	Texture* textureSpace = new Texture("test3.png", true);
	Texture* texturePlayer = new Texture("playertest.png");
	Sprite* planet = new Sprite(-4.0f, -4.0f, 2.0f, 2.0f, texturePlanet, 1.0f);
	Sprite* space = new Sprite(-16.0f, -9.0f, 32.0f, 18.0f, textureSpace, 0.0f);
	Sprite* player = new Sprite(0.0f, 0.0f, 6.0f, 2.0f, texturePlayer, 2.0f);

	Group planetPlayerer;
	planetPlayerer.add(planet);
	planetPlayerer.add(player);

	layer->add(planetPlayerer);
	layer->add(space);

	Timer* timer = new Timer();
	Timer* timerPlanetPlayer = new Timer();
	int fps = 0;
	while (!display->closed())
	{
		display->clear();
		fps++;

		if (timerPlanetPlayer->elapsed() >= 1.0f/60.0f)
		{
			planetPlayerer.rotate(0.1f);
			timerPlanetPlayer->reset();
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