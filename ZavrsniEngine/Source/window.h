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

		bool _Keys[MAX_KEYS];
		bool _KeysPressed[MAX_KEYS];
		bool _MouseButtons[MAX_BUTTONS];
		bool _MouseButtonsPressed[MAX_BUTTONS];
		bool _requireInputReset;

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
		
		bool getKeyPressed(int key);
		bool getKey(int key);
		bool getMouseButtonPressed(int mouseButton);
		bool getMouseButton(int mouseButton);

		inline int getWidth() { return _width; }
		inline int getHeight() { return _height; }
	private:
		void init();
		void resetInputPress();

		friend void cursor_position(GLFWwindow* window, double xpos, double ypos);
		friend void window_resize(GLFWwindow* window, int width, int height);
		friend void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void mouse_button_pressed(GLFWwindow* window, int button, int action, int mods);
	};
}