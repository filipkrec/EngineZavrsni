#pragma once
#include "../Math/Math.h"

class Camera
{
private:
    static Camera* instance;
    math::Matrix4 _viewMatrix;
    math::Vector2 _offset;
    Camera();

public:
    static Camera* getInstance();
    void move(const math::Vector2& vector);
    inline const math::Matrix4& getViewMatrix() const { return _viewMatrix; }
    inline const math::Vector2& getOffset() const { return _offset; }
};