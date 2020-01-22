#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include "Source/Graphics/Shaders/Shader.h"
#include "Source/Graphics/Renderer.h"
#include "Source/Graphics/Sprite.h"
#include "Source/Graphics/Layer.h"
#include <iostream>
#define AT_JOB 0

#if 1
int main()
{
	using namespace engine;
	using namespace graphics;
	Window* display = new Window("Display", 1280, 920);
	Layer* layer = new Layer();
	Texture* texture = new Texture("test.png");
	Sprite* sprite = new Sprite(0.0f, 0.0f, 0.5f, 0.5f, 0xffffff00);
	Sprite* sprite2 = new Sprite(-1.0f, 0.0f, 0.5f, 0.5f, texture);
	Sprite* sprite3 = new Sprite(-1.0f, -1.0f, 0.5f, 0.5f, 0xff00ffff);
	Sprite* sprite4 = new Sprite(0.0f, -1.0f, 0.5f, 0.5f, texture);
	layer->add(sprite);
	layer->add(sprite2);
	layer->add(sprite3);
	layer->add(sprite4);

	while (!display->closed())
	{
		display->clear();
		
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
	using namespace math;

	Window window("Sparky!", 960, 540);

	Layer* layer = new Layer();
	layer->_shader->enable();
	glActiveTexture(GL_TEXTURE0);
	Texture texture("test.png");
	glBindTexture(GL_TEXTURE_2D, texture.getId());
	layer->_shader->enable();
	while (!window.closed())
	{
		window.clear();

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(0, 0);
		glTexCoord2f(0, 1);
		glVertex2f(0, 4);
		glTexCoord2f(1, 1);
		glVertex2f(4, 4);
		glTexCoord2f(1, 0);
		glVertex2f(4, 0);
		glEnd();

		window.update();
	}

	return 0;
}
#endif