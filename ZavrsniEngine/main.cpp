#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include "Source/Graphics/Layer.h"
#include <iostream>
#define AT_JOB 0


int main()
{
	using namespace engine;
	using namespace graphics;
	Window* display = new Window("Display", 1280, 920);
	Sprite* sprite = new Sprite(0.0f, 0.0f, 1.0f, 1.0f, 0xff00ff00);
	Sprite* sprite2 = new Sprite(-1.0f, 0.0f, 1.0f, 1.0f, 0xfffc2011);
	Sprite* sprite3 = new Sprite(0.0f, -1.0f, 1.0f, 1.0f, 0xffffff00);
	Layer* layer = new Layer(math::Matrix4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
	layer->add(sprite);
	layer->add(sprite2);
	layer->add(sprite3);

	while (!display->closed())
	{
		display->clear();
		
		layer->render();

		display->update();
	}
}