#ifndef MATHS_H
#define MATHS_H

#include <cmath>

#include "Vectors\Vec2.h"
#include "Vectors\Vec3.h"
#include "Vectors\Vec4.h"

#include "Matrices\Mat2.h"
#include "Matrices\Mat3.h"
#include "Matrices\Mat4.h"

#include "MathsConstants.h"

class Maths
{
public:
	template<typename T>
	static float dot(const T& left, const T& right);

	static Vec3 cross(const Vec3& left, const Vec3& right);

	template<typename T>
	static T project(const T& left, const T& right);

	template<typename T>
	static T normalize(const T& v);

	template<typename T>
	static float length(const T& v);

	template<typename T>
	static T transpose(const T& m);

	static float det(const Mat2& m);
	static float det(const Mat3& m);
	static float det(const Mat4& m);

	static float toRadians(float deg);
	static float toDegrees(float rad);
};

// --------------------- IMPLEMENTATION ---------------------

template<typename T>
float Maths::dot(const T& left, const T& right)
{
	float sum = 0.0f;
	for (int i = 0; i < left.length(); i++)
		sum += left[i] * right[i];
	return sum;
}

template<typename T>
inline T Maths::project(const T & left, const T & right)
{
	float dlr = dot<T>(left, right);
	float drr = dot<T>(right, right);
	return (dlr / drr) * right;
}

template<typename T>
T Maths::normalize(const T& v)
{
	return v / length<T>(v);
}

template<typename T>
float Maths::length(const T & v)
{
	float sum = 0.0f;
	for (int i = 0; i < v.length(); i++)
		sum += v[i] * v[i];
	return sqrt(sum);
}

template<typename T>
T Maths::transpose(const T& m)
{
	T ret;
	const int side = m.getSideLength();
	for (int r = 0; r < side; r++)
		for (int c = 0; c < side; c++)
			ret[c][r] = m[r][c];
	return ret;
}

#endif
