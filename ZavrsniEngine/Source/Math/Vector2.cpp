#include "Vector2.h"

namespace math {
	Vector2::Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2::Vector2(const float& x, const float& y)
	{
		this->x = x;
		this->y = y;
	}

	float Vector2::distanceFrom(const Vector2& other) const
	{
		return sqrtf(powf((x - other.x), 2) + powf((y - other.y), 2));
	}

	Vector2& Vector2::add(const Vector2& other)
	{
		y += other.y;
		x += other.x;

		return *this;
	}

	Vector2& Vector2::subtract(const Vector2& other)
	{
		y -= other.y;
		x -= other.x;

		return *this;
	}

	Vector2& Vector2::multiply(const Vector2& other)
	{
		y *= other.y;
		x *= other.x;

		return *this;
	}

	Vector2& Vector2::divide(const Vector2& other)
	{
		y /= other.y;
		x /= other.x;

		return *this;
	}

	Vector2& Vector2::divide(const float& scalar)
	{
		y /= scalar;
		x /= scalar;

		return *this;
	}


	Vector2 operator+(Vector2 left, const Vector2& right)
	{
		return left.add(right);
	}

	Vector2 operator-(Vector2 left, const Vector2& right)
	{
		return left.subtract(right);
	}

	Vector2 operator*(Vector2 left, const Vector2& right)
	{
		return left.multiply(right);
	}

	Vector2 operator/(Vector2 left, const Vector2& right)
	{
		return left.divide(right);
	}

	Vector2 operator/(Vector2 left, const float& scalar)
	{
		return left.divide(scalar);
	}

	Vector2& Vector2::operator+=(const Vector2& right)
	{
		return add(right);
	}

	Vector2& Vector2::operator-=(const Vector2& right)
	{
		return subtract(right);
	}

	Vector2& Vector2::operator*=(const Vector2& right)
	{
		return multiply(right);
	}

	Vector2& Vector2::operator/=(const Vector2& right)
	{
		return divide(right);
	}

	bool Vector2::operator==(const Vector2& right)
	{
		return (right.x == x && right.y == y);
	}

	bool Vector2::operator!=(const Vector2& right)
	{
		return (right.x != x && right.y != y);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector2& vector2)
	{
		return stream << "Vector2: " << vector2.x << "," << vector2.y;
	}


	Vector2 Vector2::calculateUnitVector(float x, float y)
	{

		float length = sqrtf((x * x) + (y * y));
		float retX = x;
		float retY = y;

		if(length > 0)
			return math::Vector2(retX / length, retY / length);
		else 
			return math::Vector2(0, 0);
	}


}