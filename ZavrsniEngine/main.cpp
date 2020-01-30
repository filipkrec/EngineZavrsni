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
	Window* display = new Window("Display", 800, 600);
	Layer* layer = new Layer(math::Matrix4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
	Texture* texturePlanet = new Texture("test2.png");
	Texture* textureSpace = new Texture("test3.png", true);
	Texture* texturePlayer = new Texture("playertest.png");

	Sprite* planet = new Sprite(-4.0f, -4.0f, 2.0f, 2.0f, texturePlanet, 1);
	Sprite* player = new Sprite(0.0f, 0.0f, 6.0f, 2.0f, texturePlayer, 2);

	Sprite* space = new Sprite(-16.0f, -9.0f, 32.0f, 18.0f, textureSpace);

	Sprite* healthBar = new Sprite(-10.0f, 5.0f, 4.0f, 0.5f, 0xff00ff00,3);
	Font* arial = new Font("arial.ttf", 16);
	arial->setScale(800.0f / 32.0f, 600.0f / 18.0f);
	Label* label = new Label("100 / 100", -10.0f, 5.0f, 0xff000000, arial, 4);

	Group planetPlayerer;
	planetPlayerer.add(planet);
	planetPlayerer.add(player);

	Group health;
	health.add(healthBar);
	health.add(label);

	health.applyTransformation(math::Matrix4::translation(math::Vector2(-7.0f, 7.0f)));

	//layer->add(label);
	layer->add(planetPlayerer);
	layer->add(health);
	layer->add(space);

	Timer* timer = new Timer();
	Timer* timerPlanetPlayer = new Timer();
	int fps = 0;
	std::string fpsstr = std::to_string(fps);
	while (!display->closed())
	{
		display->clear();
		fps++;

		if (timerPlanetPlayer->elapsed() >= 1.0f / 60.0f)
		{
			planetPlayerer.rotate(0.1f);
			timerPlanetPlayer->reset();
		}

		if (timer->elapsed() >= 1.0f)
		{
			fpsstr = std::to_string(fps);
			label->setText(fpsstr);
			timer->reset();
			fps = 0;
		}
		layer->render();
		display->update();
	}
}
#endif