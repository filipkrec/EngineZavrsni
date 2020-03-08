#include "Window.h"

namespace engine {
	void window_resize(GLFWwindow* window, int width, int height);
	void cursor_position(GLFWwindow* window, double xpos, double ypos);
	void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_button_pressed(GLFWwindow* window, int button, int action, int mods);

	Window::Window(const char* title, int width, int height, bool fullscreen)
		:_title(title),_width(width),_height(height),_fullscreen(fullscreen),_vsync(false),_showCursor(true)
	{
		init();
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::update()
	{
		glfwPollEvents();
		glfwGetFramebufferSize(_window, &_width, &_height);
		glfwSwapBuffers(_window);
	}

	void Window::clearInput()
	{
		if (_requireInputReset == true)
		{
			_requireInputReset = false;
			resetInputPress();
		}
	}

	bool Window::closed() const
	{
		return glfwWindowShouldClose(_window);
	}

	void Window::close()
	{
		return glfwSetWindowShouldClose(_window,1);
	}

	void Window::init()
	{
		if (!glfwInit())
		{
			std::cout << "Failed Init" << std::endl;
			glfwTerminate();
			return;
		}

		if(_fullscreen)
			_window = glfwCreateWindow(_width, _height, _title, glfwGetPrimaryMonitor(), NULL); //fullscreen
		else
			_window = glfwCreateWindow(_width, _height, _title, NULL, NULL); //windowed

		if (!_window)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(_window);
		glfwSetWindowUserPointer(_window, this);

		glfwSetFramebufferSizeCallback(_window, window_resize); //on size change
		glfwSetCursorPosCallback(_window, cursor_position); //on cursor position change

		glfwSetKeyCallback(_window, key_pressed);
		glfwSetMouseButtonCallback(_window, mouse_button_pressed);
		//postavljanje svih input booleana na false
		resetInputPress();
		memcpy(_keysPressed, _keys, MAX_KEYS);
		memcpy(_mouseButtonsPressed, _mouseButtons, MAX_BUTTONS);

		glfwSwapInterval(0); //vsync
		_vsync = false;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //enable texture transparency 

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not initialize GLEW" << std::endl;
			return;
		}

	}

	bool Window::toggleVsync()
	{
		if (_vsync == false)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		return _vsync = !_vsync;
	}

	bool Window::toggleCursor()
	{
		if (_showCursor == true)
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		
		return _showCursor = !_showCursor;
	}

	void Window::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void cursor_position(GLFWwindow* window, double xpos, double ypos)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->_mouseX = xpos;
		win->_mouseY = ypos;
	}

	void Window::getMousePosition(math::Vector2& point) const
	{
		
		point.x = (_mouseX / _width) * 32 - 16 - Camera::getInstance()->getOffset().x;
		point.y = -((_mouseY / _height) * 18 - 9) - Camera::getInstance()->getOffset().y;
	}

	void Window::resetInputPress()
	{
		for (int i = 0; i < MAX_KEYS; ++i)
			_keys[i] = false;
		for (int i = 0; i < MAX_BUTTONS; ++i)
			_mouseButtons[i] = false;
	}

	bool Window::getKeyPressed(int key) const
	{
		return _keysPressed[key];
	}

	bool Window::getKey(int key) const
	{
		return _keys[key];
	}

	bool Window::getMouseButtonPressed(int mouseButton) const
	{
		return _mouseButtonsPressed[mouseButton];
	}

	bool Window::getMouseButton(int mouseButton) const
	{
		return _mouseButtons[mouseButton];
	}


	void window_resize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->_height = height;
		win->_width = width;
	}

	void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS)
		{
			win->_keys[key] = true;
			win->_keysPressed[key] = true;
			win->_requireInputReset = true;
		}
		else if (action == GLFW_RELEASE)
		{
			win->_keysPressed[key] = false;
		}
	}

	void mouse_button_pressed(GLFWwindow* window, int button, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS)
		{
			win->_mouseButtons[button] = true;
			win->_mouseButtonsPressed[button] = true;
			win->_requireInputReset = true;
		}
		else if (action == GLFW_RELEASE)
		{
			win->_mouseButtonsPressed[button] = false;
		}
	}
}