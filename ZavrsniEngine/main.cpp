#include <windows.system.h>
#include "Source/Math/Math.h"
#include "Source/window.h"
#include <iostream>

int main()
{
	using namespace engine;
	Window* display = new Window("Display", 1280, 920);

	while (display)
	{
		display->update();
		
		if (display->getKey(GLFW_KEY_A))
		{
			std::cout << "A" << std::endl;
		}
		/*
		if (display->getKeyPressed(GLFW_KEY_A))
		{
			std::cout << "a" << std::endl;
		}
		*/

		if (display->getMouseButton(GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "LMB" << std::endl;
		}

		/*
		if (display->getMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "l" << std::endl;
		}
		*/

		display->clear();
	}

	system("PAUSE");
}