#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include <iostream>

int main()
{
	using namespace engine;
	Window* display = new Window("Display", 1280, 920);
	double x, y;
	while (display)
	{
		display->update();
		display->getMousePosition(x, y);
		std::cout << x << "," << y << std::endl;
		std::cout << display->getHeight() << "," << display->getWidth() << std::endl;
		display->clear();
	}

	system("PAUSE");
}