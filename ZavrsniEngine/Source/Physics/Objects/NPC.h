#pragma once
#include "Actor.h"

namespace objects
{
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


		inline const math::Vector2& getMoveDirection() const { return _moveDirection; }
		void setMoveDirection(const math::Vector2& direction);

		inline const graphics::Sprite* getLookingAt() const { return _lookingAt; }
		void setLookingAt(graphics::Sprite* sprite);
	};
}