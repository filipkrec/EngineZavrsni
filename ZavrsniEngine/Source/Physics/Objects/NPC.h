#pragma once
#include "Actor.h"

namespace objects
{
	enum class AIState {
		AI_STATE_ALERT,
		AI_STATE_TARGETING,
		AI_STATE_DEFENSIVE,
		AI_STATE_PASSIVE
	};

	class NPC :public Actor
	{
	private:
		float _rotationGoal;
		graphics::Sprite* _lookingAt;
		math::Vector2 _moveDirection;
	protected:
		engine::Timer _NPCTimer;
		AIState _AIState;
		float _reactionDelay;
		std::vector<Allegiance> _enemyAllegiances;
	public:
		static std::vector<math::Vector2> directionsAll;

		NPC();
		virtual ~NPC() {};
		NPC(GameObject gameObject, unsigned int health, float movementSpeed);
		virtual void processAI();
		virtual void init();

		void onHit(const Weapon* weapon) override;

		void moveInDirection();
		void lookAt();
		void process(const engine::Window& window);

		void addEnemyAllegiance(Allegiance allegiance);

		inline const math::Vector2& getMoveDirection() const { return _state != ActorState::STATE_DEAD ? _moveDirection : math::Vector2(0.0f,0.0f); }
		void setMoveDirection(const math::Vector2& direction);

		inline const float getReactionDelay() const { return _reactionDelay; }
		void setReactionDelay(float delay);

		inline const graphics::Sprite* getLookingAt() const { return _state != ActorState::STATE_DEAD ? _lookingAt : nullptr; }
		void setLookingAt(graphics::Sprite* sprite);

		void setAIState(AIState state);
		inline const AIState getAIState() const { return _state != ActorState::STATE_DEAD ? _AIState : AIState::AI_STATE_PASSIVE; }
	};
}