#include "window.h"

namespace engine{
	void window_resize(GLFWwindow* window, int width, int height);
	void cursor_position(GLFWwindow* window, double xpos, double ypos);

	Window::Window(const char* title, int width, int height)
	{
		_title = title;
		_width = width;
		_height = height;
		_closed = false;

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

	void Window::clear() const
	{
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


	void window_resize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->_height = height;
		win->_width = width;
	}
}