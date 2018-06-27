#include "Maths.h"

Vec3 Maths::cross(const Vec3 & left, const Vec3 & right)
{
	return Vec3(left.y*right.z - left.z*right.y,
				left.z*right.x - left.x*right.z,
				left.x*right.y - left.y*right.x);
}

float Maths::det(const Mat2 & m)
{
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

float Maths::det(const Mat3 & m)
{
	Mat2 m0(m[1][1], m[2][1], m[1][2], m[2][2]);
	Mat2 m1(m[0][1], m[2][1], m[0][2], m[2][2]);
	Mat2 m2(m[0][1], m[1][1], m[0][2], m[1][2]);
	return det(m0) + det(m1) + det(m2);
}

float Maths::det(const Mat4 & m)
{
	float sum = 0.0f;
	const int side = m.getSideLength();
	for (int i = 0; i < side; i++)
	{
		Mat3 mIn;
		int indexC = 0;
		for (int c = 0; c < side; c++)
		{
			if (c != i)
			{
				for (int r = 0; r < side; r++)
					mIn[indexC][r - 1] = m[c][r];
				indexC++;
			}
		}
		sum += det(mIn);
	}

	return sum;
}

float Maths::toRadians(float deg)
{
	return deg * MathsConstatns::PI / 180.0f;
}

float Maths::toDegrees(float rad)
{
	return rad * 180.0f / MathsConstatns::PI;
}