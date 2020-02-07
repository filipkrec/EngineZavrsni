#pragma once
#include <GL/glew.h>
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h"
#include <iostream>


namespace engine {

#define MAX_KEYS 348
#define MAX_BUTTONS 8
	class Window
	{
	private:
		const char* _title;
		int _width, _height;
		GLFWwindow* _window;

		bool _keys[MAX_KEYS];
		bool _keysPressed[MAX_KEYS];
		bool _mouseButtons[MAX_BUTTONS];
		bool _mouseButtonsPressed[MAX_BUTTONS];
		bool _requireInputReset;
		
		bool _fullscreen;
		bool _vsync;
		bool _showCursor;
		float _mouseX, _mouseY;
	public:
		Window(const char* title, int width, int height, bool fullscreen = false);
		~Window();
		void close();
		bool closed() const;
		void update();
		void clear();
		bool toggleVsync();
		bool toggleCursor();
		void getMousePosition(float& x, float& y) const;

		bool getKeyPressed(int key) const;
		bool getKey(int key) const;
		bool getMouseButtonPressed(int mouseButton) const;
		bool getMouseButton(int mouseButton) const;

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