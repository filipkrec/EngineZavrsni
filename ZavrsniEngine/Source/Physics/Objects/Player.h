#pragma once
#include "Actor.h"
#include "../../Window.h"

namespace objects {
	class Player :public Actor
	{
	protected:
		int _keyUp;
		int _keyDown;
		int _keyRight;
		int _keyLeft;
		int _keyPickup;
		float _movementForce;
	public:
		Player();
		Player(GameObject gameObject,unsigned int _health,float _movementSpeedMax);

		const math::Vector2 getVectorToMouse(const engine::Window& window) const;

		virtual void animate();
		virtual void processState();
		virtual void processInput(const engine::Window&  window); //process movement, process rotation (following cursor)
		virtual void process(const engine::Window& window); 

		void setKeys(int keyPickup = 0,int keyUp = GLFW_KEY_W, int keyDown = GLFW_KEY_S, int keyRight = GLFW_KEY_D, int keyLeft = GLFW_KEY_A);
		void setForce(float movementForce = 500.0f);

		friend class LevelAssetManager;
	};
}