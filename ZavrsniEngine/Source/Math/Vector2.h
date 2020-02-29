#pragma once
#include <iostream>
#include "MathUtil.h"

namespace math {
	struct Vector2
	{
		float x, y;

		Vector2();
		Vector2(const float &x, const float &y);

		float distanceFrom(const Vector2& other) const;

		Vector2& add(const Vector2& other);
		Vector2& subtract(const Vector2& other);
		Vector2& multiply(const Vector2& other);
		Vector2& divide(const Vector2& other);

		Vector2& divide(const float& scalar);
		Vector2& multiply(const float& scalar);
		static Vector2 calculateUnitVector(math::Vector2 vec);
		static float getAngleBetween(float currentRotation, const math::Vector2& vectorDistanceToOther);

		friend Vector2 operator+(Vector2 left, const Vector2& right);
		friend Vector2 operator-(Vector2 left, const Vector2& right);
		friend Vector2 operator*(Vector2 left, const Vector2& right);
		friend Vector2 operator/(Vector2 left, const Vector2& right);

		friend Vector2 operator/(Vector2 left, const float& scalar);
		friend Vector2 operator*(Vector2 left, const float& scalar);

		friend bool operator>=(Vector2 left, const Vector2& right);
		friend bool operator<=(Vector2 left, const Vector2& right);
		friend bool operator>(Vector2 left, const Vector2& right);
		friend bool operator<(Vector2 left, const Vector2& right);


		Vector2& operator+=(const Vector2& right);
		Vector2& operator-=(const Vector2& right);
		Vector2& operator*=(const Vector2& right);
		Vector2& operator/=(const Vector2& right);


		bool operator==(const Vector2& right) const;
		bool operator!=(const Vector2& right) const;

		friend std::ostream& operator<<(std::ostream& stream, const Vector2& vector2);
	};
}