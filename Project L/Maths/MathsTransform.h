#ifndef MATHS_TRANSFORM_H
#define MATHS_TRANSFORM_H

#include "Maths.h"

class MathsTransform
{
public:
	static Mat4 perspective(float fov, float aspectRatio, float zNear, float zFar);
	static Mat4& perspective(Mat4& m, float fov, float aspectRatio, float zNear, float zFar);
	static Mat4 orthographic(float left, float right, float top, float bottom, float zNear, float zFar);
	static Mat4& orthographic(Mat4& m, float left, float right, float top, float bottom, float zNear, float zFar);

	static Mat4 initCamera(const Vec3& direction, const Vec3& worldUp, const Vec3& position = { 0.0f, 0.0f, 0.0f });
	static Mat4& initCamera(Mat4& m, const Vec3& direction, const Vec3& worldUp, const Vec3& position);
	static Mat4 initCamera(const Vec3& right, const Vec3& up, const Vec3& forward, const Vec3& position = { 0.0f, 0.0f, 0.0f });
	static Mat4& initCamera(Mat4& m, const Vec3& right, const Vec3& up, const Vec3& forward, const Vec3& position);

	static Vec3 rotate(const Vec3& v, float angle, const Vec3& axis);
	static Vec3 rotate(const Vec3& v, float x, float y, float z);
	static Mat3 rotate(const Mat3& m, float x, float y, float z);

	static Vec4 rotate(const Vec4& v, float angle, const Vec3& axis);
	static Vec4 rotate(const Vec4& v, float x, float y, float z);
	static Mat4 rotate(const Mat4& m, float x, float y, float z);
	static Mat4 rotationMat(float x, float y, float z);

	static Mat4 lookAt(const Vec3& target, const Vec3& position);
	static Mat4& lookAt(Mat4& m, const Vec3& target, const Vec3& position);
	static Mat4 lookIn(const Vec3& direction);
	static Mat4& lookIn(Mat4& m, const Vec3& direction);

	static Mat4 scale(float x, float y, float z);
	static Mat4& scale(Mat4& m, float x, float y, float z);

	static Mat4 translate(float x, float y, float z);
	static Mat4& translate(Mat4& m, float x, float y, float z);

	static Mat4 getTranslationMat(const Mat4& m);
};

#endif
