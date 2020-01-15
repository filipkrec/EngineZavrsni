#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include <iostream>

int main()
{
	using namespace engine;
	using namespace graphics;
	Window* display = new Window("Display", 1280, 920);
	Shader* shader = new Shader("Source/Graphics/Shaders/Basic.vert", "Source/Graphics/Shaders/Basic.frag");
	shader->enable();
	Renderer renderer;
	Sprite* sprite = new Sprite(0.0f, 0.0f, 1.0f, 1.0f, 0xff00ff00);

	while (display)
	{
		display->clear();

		renderer.begin();
		renderer.submit(sprite);
		renderer.end();
		renderer.flush();

		display->update();
	}

	system("PAUSE");
}