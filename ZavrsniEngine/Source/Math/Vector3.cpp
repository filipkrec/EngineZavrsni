#include "Vector3.h"

namespace math {
	Vector3::Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vector3::Vector3(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3& Vector3::add(const Vector3& other)
	{
		y += other.y;
		x += other.x;
		z += other.z;

		return *this;
	}

	Vector3& Vector3::subtract(const Vector3& other)
	{
		y -= other.y;
		x -= other.x;
		z -= other.z;

		return *this;
	}

	Vector3& Vector3::multiply(const Vector3& other)
	{
		y *= other.y;
		x *= other.x;
		z *= other.z;

		return *this;
	}

	Vector3& Vector3::divide(const Vector3& other)
	{
		y /= other.y;
		x /= other.x;
		z /= other.z;

		return *this;
	}

	Vector3 operator+(Vector3 left, const Vector3& right)
	{
		return left.add(right);
	}

	Vector3 operator-(Vector3 left, const Vector3& right)
	{
		return left.subtract(right);
	}

	Vector3 operator*(Vector3 left, const Vector3& right)
	{
		return left.multiply(right);
	}

	Vector3 operator/(Vector3 left, const Vector3& right)
	{
		return left.divide(right);
	}

	Vector3& Vector3::operator+=(const Vector3& right)
	{
		return add(right);
	}

	Vector3& Vector3::operator-=(const Vector3& right)
	{
		return subtract(right);
	}

	Vector3& Vector3::operator*=(const Vector3& right)
	{
		return multiply(right);
	}

	Vector3& Vector3::operator/=(const Vector3& right)
	{
		return divide(right);
	}

	bool Vector3::operator==(const Vector3& right)
	{
		return (right.x == x && right.y == y && right.z == z);
	}

	bool Vector3::operator!=(const Vector3& right)
	{
		return (right.x != x && right.y != y && right.z != z);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector3& Vector3)
	{
		return stream << "Vector3: " << Vector3.x << "," << Vector3.y << "," << Vector3.z;
	}


}