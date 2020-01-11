#include <windows.system.h>
#include "Source/Math/Math.h"
#include <iostream>

int main()
{
	using namespace math;

	float values1[] =
	{ 1.0f,2.0f,3.0f,4.0f,
		5.0f,6.0f,7.0f,8.0f,
		9.0f,10.0f,11.0f,12.0f,
		13.0f,14.0f,15.0f,16.0f };

	float values2[16];
	for (int i = 0; i < 16; ++i)
	{
		values2[i] = values1[15 - i];
	}

	Matrix4 matrix1(values1);
	Matrix4 matrix2(values2);

	std::cout << matrix1 << std::endl;
	std::cout << matrix1 * matrix2 << std::endl;


	system("PAUSE");
}