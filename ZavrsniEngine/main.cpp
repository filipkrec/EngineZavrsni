#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include "Source/Graphics/Layer.h"
#include "Source/Util/Timer.h"
#include "Source/Graphics/Group.h"
#include "Source/Physics/Hitbox.h"
#include <iostream>
#define AT_JOB 0


#if 1
int main()
{
	using namespace engine;
	using namespace graphics;
	using namespace physics;
	using namespace math;
	Window* display = new Window("Display", 800, 600);
	Layer* layer = new Layer(Matrix4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
	Texture* texturePlanet = new Texture("test2.png");
	Texture* textureSpace = new Texture("test3.png", true);
	Texture* texturePlayer = new Texture("playertest.png");

	Sprite* planet = new Sprite(-4.0f, -4.0f, 2.0f, 2.0f, texturePlanet, 1);
	Sprite* player = new Sprite(0.0f, 0.0f, 6.0f, 2.0f, texturePlayer, 2);

	Sprite* space = new Sprite(-16.0f, -9.0f, 32.0f, 18.0f, textureSpace);

	Sprite* healthBar = new Sprite(-10.0f, 5.0f, 4.0f, 0.5f, 0xff00ff00, 3);

	Sprite* hitSprite = new Sprite(-5.0f, 5.0f, 4.0f, 4.0f, 0xff0000ff, 5);

	Font* arial = new Font("arial.ttf", 16);
	arial->setScale(800.0f / 32.0f, 600.0f / 18.0f);
	Label* label = new Label("100 / 100", -10.0f, 5.0f, 0xff000000, arial, 4);
	Label* label2 = new Label("Puno textaPuno textaPuno textaPuno textaPuno textaPuno textaPuno textaPuno textaPuno texta", -14.0f, 2.0f, 0xff0000ff, arial, 4);

	Group health;
	health.add(healthBar);
	health.add(label);

	Hitbox hitbox(hitSprite, Shape::SQUARE, 4.0f);
	Hitbox hitboxPlanet(planet, Shape::SQUARE, 2.0f);

	health.applyTransformation(Matrix4::scale(Vector2(1.5f, 1.5f)));


	layer->add(planet);
	layer->add(player);
	layer->add(health);
	layer->add(space);
	layer->add(hitSprite);
	layer->add(label2);

	Timer* timer = new Timer();
	Timer* timerPlanetPlayer = new Timer();
	int fps = 0;
	std::string fpsstr = std::to_string(fps);

	double x, y;
	while (!display->closed())
	{
		display->clear();
		fps++;
		display->getMousePosition(x, y);
		x = (x / 800) * 32 - 16;
		y = - ((y / 600) * 18 - 9);

		if (timerPlanetPlayer->elapsed() >= 1.0f / 60.0f)
		{
			std::cout << "X:" << x << " Y:" << y << std::endl;
			planet->RotatePosition(1);
			timerPlanetPlayer->reset();
		}

		if (timer->elapsed() >= 1.0f)
		{
			fpsstr = std::to_string(fps);
			label->setText(fpsstr);
			timer->reset();
			fps = 0;
		}

		
		if (hitbox.isHit(Vector2(x, y)))
			hitSprite->setColor(0xff00ff00);
		else if (hitbox.isHit(hitboxPlanet))
			hitSprite->setColor(0xff00ffff);
		else 
			hitSprite->setColor(0xff0000ff);

		layer->render();
		display->update();
	}
}
#endif