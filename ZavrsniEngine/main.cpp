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
	Layer* layer = new Layer(math::Matrix4::orthographic(-16.0f,16.0f,-9.0f,9.0f,-1.0f,1.0f));
	Texture* texturePlanet = new Texture("test2.png");
	Texture* textureSpace = new Texture("test3.png", true);
	Texture* texturePlayer = new Texture("playertest.png");
	layer->add(new Sprite(-4.0f, -4.0f, 2.0f, 2.0f, texturePlanet,1.0f));
	layer->add(new Sprite(-16.0f, -9.0f, 32.0f, 18.0f, textureSpace,0.0f));
	layer->add(new Sprite(0.0f, 0.0f, 6.0f, 2.0f, texturePlayer,2.0f));

	Timer* timer = new Timer();
	Timer* timerPosition = new Timer();
	Timer* timerBackground = new Timer();
	struct PlayerState {
		enum state {
			UP,
			DOWN,
			LEFT,
			RIGHT
		};
		state currentState;
	};
	PlayerState state;
	state.currentState = state.RIGHT;
	int fps = 0;
	while (!display->closed())
	{
		display->clear();
		fps++;
		if (display->getKey(GLFW_KEY_RIGHT))
		{
			if (state.currentState != state.RIGHT)
			{
				switch (state.currentState)
				{
				case state.UP:
					break;
				case state.DOWN:
					break;
				case state.LEFT:
					break;
				}
			}
		}

		if (timerBackground->elapsed() >= 1.0f/60.0f)
		{
			//testBackground->Rotate(0.1);
			timerBackground->reset();
		}

		if (timerPosition->elapsed() >= 0.01f)
		{
			//test->RotatePosition(1);
			timerPosition->reset();
		}
		//test->Rotate(1);

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