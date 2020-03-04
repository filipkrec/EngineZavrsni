#pragma once
#include "Actor.h"

namespace objects
{
	enum AIState {
		AI_STATE_ALERT,
		AI_STATE_TARGETING,
		AI_STATE_DEFENSIVE,
		AI_SATE_PASSIVE
	};

	class NPC :public Actor
	{
		bool _friendly;
		math::Vector2 _moveDirection;
		graphics::Sprite* _lookingAt;
	public:
		static std::vector<math::Vector2> directionsAll;

		NPC();
		virtual ~NPC() {};
		NPC(GameObject gameObject, unsigned int health, float movementSpeed);
		virtual void animate() {};
		virtual void processState() {};
		virtual void processAI() {};
		virtual void init();
		void moveInDirection();
		void lookAt();
		void process(const engine::Window& window);


		inline const math::Vector2& getMoveDirection() const { return _state != STATE_DEAD ? _moveDirection : math::Vector2(0.0f,0.0f); }
		void setMoveDirection(const math::Vector2& direction);

		inline const graphics::Sprite* getLookingAt() const { return _state != STATE_DEAD ? _lookingAt : nullptr; }
		void setLookingAt(graphics::Sprite* sprite);
	};
}