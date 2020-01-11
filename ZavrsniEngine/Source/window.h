#pragma once
#include <GL/glew.h>
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h"
#include <iostream>


namespace engine {

#define MAX_KEYS 1024
#define MAX_BUTTONS 32
	class Window
	{
	private:
		const char* _title;
		int _width, _height;
		GLFWwindow* _window;
		bool _closed;

		bool _Vsync;
		double _mouseX, _mouseY;
	public:
		Window(const char* title, int width, int height);
		~Window();
		bool closed() const;
		void update();
		void clear() const;
		bool toggleVsync();
		void getMousePosition(double& x, double& y);

		inline int getWidth() { return _width; }
		inline int getHeight() { return _height; }
	private:
		void init();
		friend void cursor_position(GLFWwindow* window, double xpos, double ypos);
		friend void window_resize(GLFWwindow* window, int width, int height);
	};
}