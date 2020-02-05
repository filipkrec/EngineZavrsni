#pragma once

#include "GameObject.h"
#include "../../Window.h"
#include "../../Util/Timer.h"

namespace physics {
	class Actor :GameObject
	{
		enum State
		{
		STATE_MOVING,
		STATE_DEAD,
		STATE_STILL
		};
		static const engine::Timer* timer;

		unsigned int _health;
		float _movementSpeed;
		bool _isAlive;
		std::vector<unsigned int> _textureIds;
		State _state;
	protected:
		void addTexture(const graphics::Texture& texture) { _textureIds.push_back(texture.getId()); }
		virtual void animate() {};
		virtual void processState() {};
		virtual void process() = 0;
	};
}