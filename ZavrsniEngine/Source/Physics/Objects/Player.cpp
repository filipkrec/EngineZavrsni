#include "Player.h"
#include "../../Math/Matrix4.cpp"

namespace physics {

	Player::Player()
		:Actor()
	{
	}

	Player::Player(GameObject gameObject, unsigned int health, float movementSpeedMax)
		:Actor(gameObject,health,movementSpeedMax,Actor::State::STATE_STILL)
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
		int pressedKeys[10];
		int pressedMouseButtons[3];
		int mouse_i = 0;
		int key_i = 0;

		float force = 500;

		//movement
		if (window.getKeyPressed(GLFW_KEY_W))
		{
			if (window.getKeyPressed(GLFW_KEY_A) != window.getKeyPressed(GLFW_KEY_D))
			{
				force = force / 1.6;
			}
			calculateColission(math::Vector3(0.0f, 1.0f, force));
		}

		if (window.getKeyPressed(GLFW_KEY_S))
		{
			if (window.getKeyPressed(GLFW_KEY_A) != window.getKeyPressed(GLFW_KEY_D))
			{
				force = force / 1.6;
			}
			calculateColission(math::Vector3(0.0f, -1.0f, force));
		}

		if (window.getKeyPressed(GLFW_KEY_A))
		{
			calculateColission(math::Vector3(-1.0f, 0.0f, force));
		}
		
		if (window.getKeyPressed(GLFW_KEY_D))
		{
			calculateColission(math::Vector3(1.0f, 0.0f, force));
		}

		if (window.getKeyPressed(GLFW_KEY_W) || window.getKeyPressed(GLFW_KEY_S) || window.getKeyPressed(GLFW_KEY_A) || window.getKeyPressed(GLFW_KEY_D))
		{
			if(_state != State::STATE_DEAD && _state != State::STATE_MOVING)
				_state = State::STATE_MOVING;
		}
		else
		{
			if (_state != State::STATE_DEAD && _state != State::STATE_STILL)
			{
				_state = State::STATE_STILL;
				calculateColission(math::Vector3(-_currentForce.x, -_currentForce.y, force)); //STOP
			}
		}

		//rotation
		if (window.getKeyPressed(GLFW_KEY_K))
		{
			float goalRotation;
			math::Vector2 mouseVector = getVectorToMouse(window);
			if (mouseVector.x != 0)
			{
				float mouseVectorMagnitude = sqrtf((mouseVector.x * mouseVector.x) + (mouseVector.y * mouseVector.y));
				goalRotation = acosf(mouseVector.x / mouseVectorMagnitude);
				goalRotation = math::toDegrees(goalRotation);
			}
			else
			{
				if (mouseVector.y > 0)
					goalRotation = 90;
				else
					goalRotation = 270;
			}

			float spriteRotation = _sprite.getRotation();
			float rotationTop = goalRotation - spriteRotation;
			float rotationBottom = spriteRotation - goalRotation;
			rotationBottom = rotationBottom < 0 ? rotationBottom + 360 : rotationBottom;
			rotationTop = rotationTop < 0 ? rotationTop + 360 : rotationTop;
			float rotationAngle = rotationBottom < rotationTop ? rotationBottom : rotationTop;

			if (rotationAngle < 1)
				_sprite.Rotate(rotationAngle);
			else if (rotationBottom < rotationTop)
				_sprite.Rotate(-1);
			else
				_sprite.Rotate(1);
		}

	}

	void Player::process(const engine::Window& window)
	{
			processInput(window);
			processState();
			animate();
			move();
	}


	const math::Vector2 Player::getVectorToMouse(const engine::Window& window) const
	{
		float x;
		float y;
		window.getMousePosition(x, y);
		float spriteX = _sprite.getPosition()->x - _sprite.getSize().x/2;
		float spriteY = _sprite.getPosition()->y - _sprite.getSize().y/2;

		return math::Vector2(x - spriteX, y - spriteY);
	}
}