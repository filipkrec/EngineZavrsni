#pragma once
#include "Actor.h"
#include "../../Window.h"

namespace physics {
	class Player :public Actor
	{
	public:
		Player();
		Player(GameObject gameObject,unsigned int _health,float _movementSpeedMax);

		const math::Vector2 getVectorToMouse(const engine::Window& window) const;

		void animate();
		void processState();
		void processInput(const engine::Window&  window);
		void process(const engine::Window& window);

		friend class LevelAssetManager;
	};
}