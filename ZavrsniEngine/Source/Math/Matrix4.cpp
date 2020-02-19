#pragma once

#include "Matrix4.h"
#include "MathUtil.h"

namespace math {

	const void Matrix4::print()
	{
		for (int i = 0; i < 16; ++i)
		{
			printf("%f ,", elements[i]);
			if ((i + 1) % 4 == 0)
			{
				printf("\n");
			}
		}
		printf("\n");
	}

	Matrix4::Matrix4()
	{
		for (int i = 0; i < 4 * 4; ++i)
		{
			elements[i] = 0.0f;
		}
	}


	Matrix4::Matrix4(float inputElements[])
	{
		for (int i = 0; i < 4 * 4; ++i)
		{
			elements[i] = inputElements[i];
		}
	}

	Matrix4::Matrix4(float diagonal)
	{
		for (int i = 0; i < 4 * 4; ++i)
		{
			elements[i] = 0.0f;
		}

		elements[0 * 4 + 0] = diagonal;
		elements[1 * 4 + 1] = diagonal;
		elements[2 * 4 + 2] = diagonal;
		elements[3 * 4 + 3] = diagonal;
	}

	Matrix4& Matrix4::multiply(const Matrix4& other)
	{
		float data[16];
		float sum;

		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				sum = 0.0f;
				for (int e = 0; e < 4; ++e)
				{
					sum += elements[x + e * 4] * other.elements[e + y * 4];
				}

				data[y * 4 + x] = sum;
			}
		}

	memcpy(elements, data, 4 * 4 * sizeof(float));
	return *this;
	}

	Vector2 Matrix4::multiply(const Vector2& other)  const
	{
		return Vector2(
			columns[0].x * other.x + columns[1].x * other.y + columns[2].x + columns[3].x,
			columns[0].y * other.x + columns[1].y * other.y + columns[2].y + columns[3].y
		);
	}

	Vector3 Matrix4::multiply(const Vector3 & other)  const
	{
		return Vector3(
			columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x,
			columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y,
			columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z
		);
	}

	Vector4 Matrix4::multiply(const Vector4& other) const
	{
		return Vector4(
			columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x * other.w,
			columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y * other.w,
			columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z * other.w,
			columns[0].w * other.x + columns[1].w * other.y + columns[2].w * other.z + columns[3].w * other.w
		);
	}


	bool Matrix4::isEmpty()
	{               
		for (int i = 0; i < 4 * 4; ++i)
		{
			if (elements[i] != 0.0f)
				return false;
		}
		return true;
	}

	Matrix4 operator*(Matrix4 left, const Matrix4& right)
	{
		Matrix4 result = left.multiply(right);
		return left;
	}
	
	Vector2 operator*(const Matrix4& left, const Vector2& right)
	{
		Vector2 result = left.multiply(right);
		return result;
	}

	Vector3 operator*(const Matrix4& left, const Vector3& right)
	{
		Vector3 result = left.multiply(right);
		return result;
	}

	Vector4 operator*(const Matrix4& left, const Vector4& right)
	{
		Vector4 result = left.multiply(right);
		return result;
	}

	Matrix4& Matrix4::operator*=(const Matrix4& other)
	{
		return multiply(other);
	}

	Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4 result;

		result.elements[3 + 3 * 4] = 1;
		result.elements[0 + 0 * 4] = 2.0f / (right - left);
		result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
		result.elements[2 + 2 * 4] = 2.0f / (near - far);

		result.elements[0 + 3 * 4] = (left + right) / (left - right);
		result.elements[1 + 3 * 4] = (bottom + top) / (bottom - top);
		result.elements[2 + 3 * 4] = (far + near) / (far - near);

		return result;
	}

	Matrix4 Matrix4::translation(const Vector2& translation)
	{
		Matrix4 result(1.0f);
		result.elements[0 + 3 * 4] = translation.x;
		result.elements[1 + 3 * 4] = translation.y;
		result.elements[2 + 3 * 4] = 0;

		return result;
	}

	Matrix4 Matrix4::scale(const Vector2& scale)
	{
		Matrix4 result(1.0f);
		result.elements[0 + 0 * 4] = scale.x;
		result.elements[1 + 1 * 4] = scale.y;
		result.elements[2 + 2 * 4] = 0;

		return result;
	}

	Vector2 Matrix4::rotate2d(const Vector2& vector, float degrees)
	{
		return rotation(degrees, math::Vector3(0, 0, 1)) * vector;
	}

	Matrix4 Matrix4::rotation(float angle, const Vector3& axis)
	{
		Matrix4 result(1.0f);

		float r = toRadians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;

		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		result.elements[0 + 0 * 4] = x * omc + c;
		result.elements[1 + 0 * 4] = y * x * omc + z * s;
		result.elements[2 + 0 * 4] = x * z * omc - y * s;

		result.elements[0 + 1 * 4] = x * y * omc - z * s;
		result.elements[1 + 1 * 4] = y * y * omc + c;
		result.elements[2 + 1 * 4] = y * z * omc + x * s;

		result.elements[0 + 2 * 4] = x * z * omc + y * s;
		result.elements[1 + 2 * 4] = y * z * omc - x * s;
		result.elements[2 + 2 * 4] = z * z * omc + c;

		return result;
	}

	std::ostream& operator<<(std::ostream& stream, const Matrix4& matrix)
	{
		for (int i = 0; i < 16; ++i)
		{
			stream << matrix.elements[i];
			if ((i + 1) % 4 == 0)
				stream << std::endl;
			else 
				stream << " ,";
		}

		return stream;
	}
}