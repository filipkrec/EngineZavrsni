#include "Player.h"

namespace objects {

	Player::Player()
		:Actor(), _keyPickup(0),_keyUp(GLFW_KEY_W), _keyDown(GLFW_KEY_S), _keyLeft(GLFW_KEY_A), _keyRight(GLFW_KEY_D)
	{
		_movementSpeed = 100;
	}

	Player::Player(GameObject gameObject, unsigned int health, float movementSpeed)
		:Actor(gameObject,health, movementSpeed,Actor::State::STATE_STILL), _keyPickup(0), _keyUp(GLFW_KEY_W), _keyDown(GLFW_KEY_S), _keyLeft(GLFW_KEY_A), _keyRight(GLFW_KEY_D)
	{
	}

	void Player::animate()
	{

	}

	void Player::processState()
	{

	}

	void Player::processInput(const engine::Window& window)
	{

		float force = (_movementSpeed * MOVEMENT_SPEED_COEFFICIENT) * _weight;

		//movement
		if (window.getKeyPressed(_keyUp))
		{
			if (window.getKeyPressed(_keyLeft) != window.getKeyPressed(_keyRight))
			{
				force = force / 1.6;
			}
			calculateColission(math::Vector3(0.0f, 1.0f, force));
		}

		if (window.getKeyPressed(_keyDown))
		{
			if (window.getKeyPressed(_keyLeft) != window.getKeyPressed(_keyRight))
			{
				force = force / 1.6;
			}
			calculateColission(math::Vector3(0.0f, -1.0f, force));
		}

		if (window.getKeyPressed(_keyLeft))
		{
			calculateColission(math::Vector3(-1.0f, 0.0f, force));
		}
		
		if (window.getKeyPressed(_keyRight))
		{
			calculateColission(math::Vector3(1.0f, 0.0f, force));
		}

		if (window.getKeyPressed(_keyUp) || window.getKeyPressed(_keyDown) || window.getKeyPressed(_keyLeft) || window.getKeyPressed(_keyRight))
		{
			if(_state != State::STATE_DEAD && _state != State::STATE_MOVING)
				_state = State::STATE_MOVING;
		}
		else
		{
			if (_state != State::STATE_DEAD && _state != State::STATE_STILL)
			{
				_state = State::STATE_STILL;
				//calculateColission(math::Vector3(-_currentForce.x, -_currentForce.y, force)); //STOP
			}
		}

		//rotation
		math::Vector2 mouseVector = getVectorToMouse(window);
		rotateToPoint(mouseVector);

		//pickup
		if (_keyPickup == 0 || window.getKey(_keyPickup))
		{
			if (!_pickUpable.empty())
			{
				pickup(*_pickUpable.back());
			}
		}

		//shoot 
		if (window.getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		{
			if(_weapon != nullptr)
			_weapon->shoot();
		}

		//reload 
		if (window.getKeyPressed(GLFW_KEY_R))
		{
			if (_weapon != nullptr)
				_weapon->reload();
		}

	}

	void Player::process(const engine::Window& window)
	{
		if (_weapon != nullptr)
		{
			_weapon->clearShots();
			moveWeapon();
		}
		processInput(window);
		processState();
		animate();
	}


	const math::Vector2 Player::getVectorToMouse(const engine::Window& window) const
	{
		float x;
		float y;
		window.getMousePosition(x, y);

		return math::Vector2(x - _boundSprite->getPosition().x , y - _boundSprite->getPosition().y);
	}


	void Player::setKeys(int keyPickup,int keyUp, int keyDown, int keyRight, int keyLeft)
	{
		_keyPickup = keyPickup;
		_keyUp = keyUp;
		_keyDown = keyDown;
		_keyRight = keyRight;
		_keyLeft = keyLeft;
	}
}