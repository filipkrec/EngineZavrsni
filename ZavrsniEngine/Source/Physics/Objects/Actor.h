#pragma once

#include "GameObject.h"
#include "../../Window.h"
#include "../../Util/Timer.h"

namespace physics {
	class Actor : public GameObject
	{
	protected:
		enum State
		{
		STATE_MOVING,
		STATE_DEAD,
		STATE_STILL
		};

		unsigned int _health;
		float _movementSpeed;
		float _movementSpeedMax;
		std::vector<unsigned int> _textureIds;
		State _state;
	protected:
		Actor() {};
		Actor(GameObject& gameObject, unsigned int health, float movementSpeedMax,const State& state) 
			: GameObject(gameObject), _health(health),_movementSpeed(0),_movementSpeedMax(movementSpeedMax),_state(state) {}
		void addTexture(const graphics::Texture& texture) { _textureIds.push_back(texture.getId()); }
		virtual void animate() {};
		virtual void processState() {};
		virtual void process(const engine::Window& window) = 0;
	};
}