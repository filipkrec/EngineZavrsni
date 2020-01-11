#include "Window.h"

namespace engine{
	void window_resize(GLFWwindow* window, int width, int height);
	void cursor_position(GLFWwindow* window, double xpos, double ypos);
	void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_button_pressed(GLFWwindow* window, int button, int action, int mods);

	Window::Window(const char* title, int width, int height)
	{
		_title = title;
		_width = width;
		_height = height;

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

	bool Window::closed() const
	{
		return glfwWindowShouldClose(_window);
	}

	void Window::init()
	{
		if (!glfwInit())
		{
			std::cout << "Failed Init" << std::endl;
			glfwTerminate();
			return;
		}
		_window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
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
		memcpy(_KeysPressed, _Keys, MAX_KEYS);
		memcpy(_MouseButtonsPressed, _MouseButtons, MAX_BUTTONS);

		glfwSwapInterval(0); //vsync
		_Vsync = false;

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
		if (_Vsync == false)
		{
			_Vsync = true;
			glfwSwapInterval(1);
		}
		else
		{
			_Vsync = false;
			glfwSwapInterval(0);
		}

		return _Vsync;
	}

	void Window::clear()
	{
		if (_requireInputReset == true)
		{
			_requireInputReset = false;
			resetInputPress();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	
    void cursor_position(GLFWwindow* window, double xpos, double ypos)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->_mouseX = xpos;
		win->_mouseY = ypos;
	}

	void Window::getMousePosition(double& x, double& y)
	{
		x = _mouseX;
		y = _mouseY;
	}

	void Window::resetInputPress()
	{
		for (int i = 0; i < MAX_KEYS; ++i)
			_Keys[i] = false;
		for (int i = 0; i < MAX_BUTTONS; ++i)
			_MouseButtons[i] = false;
	}

	bool Window::getKeyPressed(int key)
	{
		return _KeysPressed[key];
	}

	bool Window::getKey(int key)
	{
		return _Keys[key];
	}

	bool Window::getMouseButtonPressed(int mouseButton)
	{
		return _MouseButtonsPressed[mouseButton];
	}

	bool Window::getMouseButton(int mouseButton)
	{
		return _MouseButtons[mouseButton];
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
			win->_Keys[key] = true;
			win->_KeysPressed[key] = true;
			win->_requireInputReset = true;
		}
		else if (action == GLFW_RELEASE)
		{
			win->_KeysPressed[key] = false;
		}
	}

	void mouse_button_pressed(GLFWwindow* window, int button, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS)
		{
			win->_MouseButtons[button] = true;
			win->_MouseButtonsPressed[button] = true;
			win->_requireInputReset = true;
		}
		else if (action == GLFW_RELEASE)
		{
			win->_MouseButtonsPressed[button] = false;
		}
	}
}