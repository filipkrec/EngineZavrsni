#pragma once

#include "GameObject.h"
#include "../../Window.h"
#include "../../Util/Timer.h"
#include "Inventory/Pickup.h"
#include "Inventory/Weapon.h"
#include "../../Math/MathUtil.h"
#include <algorithm>
#define MOVEMENT_SPEED_COEFFICIENT 0.05

namespace objects {
	enum class ActorState
	{
		STATE_MOVING,
		STATE_DEAD,
		STATE_STILL
	};

	class Actor : public GameObject
	{
	protected:
		engine::Timer _actorTimer;
		engine::Timer _animationTimer;



		unsigned int _health;
		float _movementSpeed;
		float _sightAngle;
		float _sightRange; 

		std::vector<std::pair<const graphics::Texture*, ActorState>> _allTextures;
		std::vector<const graphics::Texture*> _stateTextures;

		std::vector<std::pair<float, ActorState>> _animationTimers;
		float _animationTime;

		std::vector<Pickup*> _pickUpable;
		std::vector<GameObject*> _sighted;

		//pathfinding
		math::Vector2 _moveToPoint;
		math::Vector2 _patrolOriginPoint;
		math::Vector2 _patrolEndPoint;
		math::Vector2 _moveToCheckPoint;
		bool _seekCheckpoint;
		bool _pointReached;
		bool _patrol;
		bool _patroling;

		void (*_onSight)(GameObject*);

		ActorState _state;
		Weapon* _weapon;
	protected:
		Actor();
		virtual ~Actor() {};
		Actor(GameObject& gameObject, unsigned int health, float _movementSpeed, const ActorState& state);
		void moveWeapon();
		void rotateToPoint(math::Vector2);

		virtual void init();
		virtual void move() override;
		virtual void pickup(Pickup& pickup);
		virtual void animate();

		void processSight();
		virtual void process(const engine::Window& window) = 0;
	public:
		void addTexture(const graphics::Texture* texture, ActorState state); //first for state = default texture
		void setAnimationTimerForState(float interval, ActorState state); //first for state = default texture

		inline const unsigned int& getHealth() const { return _health; }
		inline const float& getMovementSpeed() const { return _movementSpeed; }
		inline const ActorState& getState() const { return _state; }
		Weapon* getWeapon();

		void setHealth(unsigned int value);
		void setMovementSpeed(float value);
		void setSight(float sightAngle, float sightRange);
		void setState(const ActorState& state);
		void setWeapon(Weapon* weapon);
		virtual void setOnSightFunction(void (*foo)(GameObject*));
		void collide(GameObject& other) override;

		void addSighted(GameObject* sighted);
		void clearObstructedSighted();
		void addPickupable(Pickup* pickupable);
		void clearPickupable();

		virtual void onHit(const objects::Weapon* weapon);

		bool objectIsInSight(const GameObject& gameObject);

		inline const math::Vector2& getMoveToPoint() const { return _moveToPoint; }
		void setMoveToPoint(const math::Vector2& point);

		inline const math::Vector2& getMoveToCheckPoint() const { return _moveToCheckPoint; }
		void setMoveToCheckPoint(const math::Vector2& point);

		inline const math::Vector2& getPatrolOriginPoint() const { return _patrolOriginPoint; }
		void setPatrolOriginPoint(const math::Vector2& point);

		inline const math::Vector2& getPatrolEndPoint() const { return _patrolOriginPoint; }
		void setPatrolEndPoint(const math::Vector2& point);

		inline const float getActorTimer() { return _actorTimer.elapsed(); }
		void resetActorTimer();

		void toggleSeekCheckpoint();
		void togglePointReached();
		void togglePatrol();
		void togglePatroling();

		inline const bool  isPointReached() const { return _state != ActorState::STATE_DEAD ? _pointReached : true; }
		inline const bool  seekCheckpoint() const { return _state != ActorState::STATE_DEAD ? _seekCheckpoint : false; }
		inline const bool  isPatrol() const { return _state != ActorState::STATE_DEAD ? _patrol : false; }
		inline const bool  isPatroling() const { return _state != ActorState::STATE_DEAD ? _patroling : false; }
	};
}