#pragma once
#include "../Math/Math.h"

class Camera
{
private:
    static Camera* instance;
    math::Matrix4 _viewMatrix;
    math::Vector2 _offset;

	bool _followPlayer;
    Camera();

public:
    static Camera* getInstance();
    void move(const math::Vector2& vector);
	void recenter();
	void followPlayer();
	void setLocation(const math::Vector2& vector);

    inline const math::Matrix4& getViewMatrix() const { return _viewMatrix; }
    inline const math::Vector2& getOffset() const { return _offset; }
	inline const bool doFollowPlayer() const { return _followPlayer; }
};
