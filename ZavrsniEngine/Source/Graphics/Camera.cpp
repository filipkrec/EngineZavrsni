#include "Camera.h"

Camera* Camera::instance = 0;

Camera* Camera::getInstance()
{
    if (instance == 0)
    {
        instance = new Camera();
    }

    return instance;
}

Camera::Camera()
    :_viewMatrix(math::Matrix4::orthographic(-16, 16, -9, 9, -1, 1)), _offset(math::Vector2(0.0f, 0.0f)), _followPlayer(false)
{
}

void Camera::move(const math::Vector2& vector)
{
    _viewMatrix *= math::Matrix4::translation(vector);
    _offset += vector;
}


void Camera::recenter()
{
	_followPlayer = false;
	_viewMatrix *= math::Matrix4::orthographic(-16, 16, -9, 9, -1, 1);
	_offset = math::Vector2(0.0f, 0.0f);
}

void Camera::followPlayer()
{
	_followPlayer = true;
}