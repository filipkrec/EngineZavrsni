#include "Vector4.h"

namespace math {
	Vector4::Vector4() 
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	Vector4::Vector4(const float& x, const float& y, const float& z, const float& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4& Vector4::add(const Vector4& other)
	{
		y += other.y;
		x += other.x;
		z += other.z;
		w += other.w;

		return *this;
	}

	Vector4& Vector4::subtract(const Vector4& other)
	{
		y -= other.y;
		x -= other.x;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	Vector4& Vector4::multiply(const Vector4& other)
	{
		y *= other.y;
		x *= other.x;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	Vector4& Vector4::divide(const Vector4& other)
	{
		y /= other.y;
		x /= other.x;
		z /= other.z;
		w /= other.w;

		return *this;
	}

	Vector4 operator+(Vector4 left, const Vector4& right)
	{
		return left.add(right);
	}

	Vector4 operator-(Vector4 left, const Vector4& right)
	{
		return left.subtract(right);
	}

	Vector4 operator*(Vector4 left, const Vector4& right)
	{
		return left.multiply(right);
	}

	Vector4 operator/(Vector4 left, const Vector4& right)
	{
		return left.divide(right);
	}

	Vector4& Vector4::operator+=(const Vector4& right)
	{
		return add(right);
	}

	Vector4& Vector4::operator-=(const Vector4& right)
	{
		return subtract(right);
	}

	Vector4& Vector4::operator*=(const Vector4& right)
	{
		return multiply(right);
	}

	Vector4& Vector4::operator/=(const Vector4& right)
	{
		return divide(right);
	}

	bool Vector4::operator==(const Vector4& right)
	{
		return (right.x == x && right.y == y && right.z == z && right.w == w);
	}

	bool Vector4::operator!=(const Vector4& right)
	{
		return (right.x != x || right.y != y || right.z != z || right.w != w);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector4& Vector4)
	{
		return stream << "Vector4: " << Vector4.x << "," << Vector4.y << "," << Vector4.z << "," << Vector4.w;
	}
}