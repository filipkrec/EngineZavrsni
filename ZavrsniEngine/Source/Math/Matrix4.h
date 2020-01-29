#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <iostream>
namespace math {
	struct Matrix4
	{
		union
		{
			Vector4 columns[4];
			float elements[4 * 4];
		};

		Matrix4();
		Matrix4(float diagonal);
		Matrix4(float inputElements[]);

		Matrix4& multiply(const Matrix4& other);
		Vector2 multiply(const Vector2& other) const;
		Vector3 multiply(const Vector3& other) const;
		Vector4 multiply(const Vector4& other) const;

		bool isEmpty();

		static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);

		static Matrix4 translation(const Vector2& translation);
		static Matrix4 rotation(float angle, const Vector3& axis);
		static Matrix4 scale(const Vector2& scale);

		friend Matrix4 operator*(Matrix4 left, const Matrix4& right);
		friend Vector2 operator*(const Matrix4& left, const Vector2& right);
		friend Vector3 operator*(const Matrix4& left, const Vector3& right);
		friend Vector4 operator*(const Matrix4& left, const Vector4& right);

		const void print();

		Matrix4& operator*=(const Matrix4& other);

		friend std::ostream& operator<<(std::ostream& stream, const Matrix4& matrix);

	};

}