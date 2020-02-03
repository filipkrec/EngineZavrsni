#pragma once

#include "Window.h"

#include "Graphics/Label.h"
#include "Graphics/Sprite.h"
#include "Graphics/Renderer.h"
#include "Graphics/Layer.h"
#include "Physics/Hitbox.h"

#include "Math/Math.h"
#include "Util/Timer.h"

namespace engine {

	class Engine
	{
	private:
		Window* _Window;
		Timer* _Timer;
	protected:
		unsigned int _FPS, _UPS;
		Engine()
			: _FPS(0), _UPS(0)
		{
		}

		virtual ~Engine()
		{
			delete _Timer;
			delete _Window;
		}

		Window* createWindow(const char* name, int width, int height, bool fullscreen)
		{
			_Window = new Window(name, width, height, fullscreen);
			return _Window;
		}

	public:
		void start()
		{
			init();
			run();
		}

	protected:
		// na poèetku izvršavanja !mora se implementirati!
		virtual void init() = 0;
		// jednom svake sekunde
		virtual void tick() {}
		// 60 puta u sekundi
		virtual void update() {}
		// Svaki loop !mora se implementirati!
		virtual void render() = 0; 

		const unsigned int getFPS() const { return _FPS; }
		const unsigned int getUPS() const { return _UPS; }
	private:
		void run()
		{
			_Timer = new Timer();
			float timer = 0.0f;
			float updateTimer = 0.0f;
			float updateTick = 1.0f / 60.0f;
			unsigned int frames = 0;
			unsigned int updates = 0;
			while (!_Window->closed())
			{
				_Window->clear();
				if (_Timer->elapsed() - updateTimer > updateTick)
				{
					update();
					updates++;
					updateTimer += updateTick;
				}
				render();
				frames++;
				_Window->update();
				if (_Timer->elapsed() - timer > 1.0f)
				{
					timer += 1.0f;
					_FPS = frames;
					_UPS = updates;
					frames = 0;
					updates = 0;
					tick();
				}
			}
		}
	};

}