#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include "Source/Graphics/Layer.h"
#include "Source/Util/Timer.h"
#include <iostream>
#define AT_JOB 0


#if 1
int main()
{
	using namespace engine;
	using namespace graphics;
	Window* display = new Window("Display", 1280, 920);
	Layer* layer = new Layer();
	Texture* texture = new Texture("test2.png");
	Texture* texture2 = new Texture("test3.png");
	Sprite* testBoja = new Sprite(-1.0f, -1.0f, 2.0f, 2.0f, texture2);
	Sprite* test = new Sprite(-0.3f, -0.3f, 0.5f, 0.5f, texture);
	layer->add(testBoja);
	layer->add(test);
	Timer* timer = new Timer();
	Timer* timerPosition = new Timer();
	int fps = 0;
	while (!display->closed())
	{
		display->clear();
		layer->render();
		display->update();
		fps++;
		if (timerPosition->elapsed() >= 0.01f)
		{
			test->RotatePosition(1);
			timerPosition->reset();
		}
		test->Rotate(1);

		if (timer->elapsed() >= 1.0f)
		{
			std::cout << fps << std::endl;
			timer->reset();
			fps = 0;
		}
	}
}
#endif

#if 0
int main()
{
	using namespace engine;
	using namespace graphics;
	Window* display = new Window("Display", 1280, 920);
	Layer* layer = new Layer();
	Texture* texture = new Texture("test.png");
	Texture* texture2 = new Texture("test2.png");
	layer->add(new Sprite(-1.0f, -1.0f, 2.0f, 2.0f, 0xffff00ff));
	for (float x = -1; x < 1; x += 0.5)
	{
		for (float y = -1; y < 1; y += 0.5)
		{
			if(((int)(y*2) + (int)(x * 2)) % 8 != 0)
				layer->add(new Sprite(x, y, 0.5f, 0.5f, texture2));
		}
	}
	Timer* timer = new Timer();
	int fps = 0;
	while (!display->closed())
	{
		display->clear();
		layer->render();
		display->update();
		fps++;
		if (timer->elapsed() >= 1.0f)
		{
			std::cout << fps << std::endl;
			timer->reset();
			fps = 0;
		}
	}
}
#endif