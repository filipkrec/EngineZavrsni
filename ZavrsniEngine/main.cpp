#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include <iostream>
#define AT_JOB 1


int main()
{
	using namespace engine;
	using namespace graphics;
	Window* display = new Window("Display", 1280, 920);

#if !AT_JOB
	Shader* shader = new Shader("Source/Graphics/Shaders/Basic.vert", "Source/Graphics/Shaders/Basic.frag");
#else
	Shader* shader = new Shader("C:/Users/fkrec/source/repos/EngineZavrsni/ZavrsniEngine/Source/Graphics/Shaders/Basic.vert", "C:/Users/fkrec/source/repos/EngineZavrsni/ZavrsniEngine/Source/Graphics/Shaders/Basic.frag");
#endif
	shader->enable();
	Renderer renderer;
	Sprite* sprite = new Sprite(0.0f, 0.0f, 0.1f, 0.1f, 0xff00ff00);
	Sprite* sprite2 = new Sprite(-1.0f, 0.0f, 0.1f, 0.1f, 0xff00ff00);
	Sprite* sprite3 = new Sprite(0.0f, -1.0f, 0.1f, 0.1f, 0xff00ff00);

	while (display)
	{
		display->clear();

		renderer.begin();
		renderer.submit(sprite);
		renderer.submit(sprite2);
		renderer.submit(sprite3);
		renderer.end();
		renderer.flush();

		display->update();
	}

	system("PAUSE");
}