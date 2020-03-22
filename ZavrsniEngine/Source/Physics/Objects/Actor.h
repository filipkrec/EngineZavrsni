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

	class Actor : public GameObject
	{
	protected:
		engine::Timer _actorTimer;
		engine::Timer _animationTimer;



		int _health;
		unsigned int _maxHealth;
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
		std::vector<math::Vector2> _path;
		bool _seekPath;
		bool _pathFinished;

		bool _pointReached;
		bool _patrol;
		bool _patroling;

		void (*_onSight)(GameObject*);

		ActorState _state;
		Weapon* _weapon;
		bool _switchedWeapon;
		math::Vector2 _weaponOffset;
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

		inline const int& getHealth() const { return _health; }
		inline const unsigned int& getMaxHealth() const { return _maxHealth; }
		inline const float& getMovementSpeed() const { return _movementSpeed; }
		const ActorState& getState() const override { return _state; }
		Weapon* getWeapon();

		void setHealth(int value);
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

		inline const math::Vector2& getWeaponOffset() const { return _weaponOffset; }
		void setWeaponOffset(const math::Vector2& weaponOffset);

		bool objectIsInSight(const GameObject& gameObject);

		inline const math::Vector2& getMoveToPoint() const { return _moveToPoint; }
		void setMoveToPoint(const math::Vector2& point);

		inline const math::Vector2& getPatrolOriginPoint() const { return _patrolOriginPoint; }
		void setPatrolOriginPoint(const math::Vector2& point);

		inline const math::Vector2& getPatrolEndPoint() const { return _patrolOriginPoint; }
		void setPatrolEndPoint(const math::Vector2& point);

		inline const float getActorTimer() { return _actorTimer.elapsed(); }
		void resetActorTimer();

		void clearPath();
		void pushPath(const math::Vector2& pathPoint);
		void popPath();
		inline const math::Vector2& getCheckpoint() const { if (!_path.empty()) return _path.back(); 
																		 else return _position; }

		void toggleSeekPath();
		void togglePathFinished();
		void togglePointReached();
		void togglePatrol();
		void togglePatroling();
		void toggleSwitchedWeapon();

		inline const bool  isPathFinished() const { return _pathFinished; }
		inline const bool  isDead() const { return _state == ActorState::STATE_DEAD; }
		inline const bool  isPointReached() const { return _state != ActorState::STATE_DEAD ? _pointReached : true; }
		inline const bool  seekPath() const { return _state != ActorState::STATE_DEAD ? _seekPath : false; }
		inline const bool  isPatrol() const { return _state != ActorState::STATE_DEAD ? _patrol : false; }
		inline const bool  isPatroling() const { return _state != ActorState::STATE_DEAD ? _patroling : false; }
		inline const bool  hasSwitchedWeapon() const { return _switchedWeapon; }
	};
}