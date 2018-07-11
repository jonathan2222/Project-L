#include "MathsTransform.h"

Mat4 MathsTransform::perspective(float fov, float aspectRatio, float zNear, float zFar)
{
	Mat4 m;
	return perspective(m, fov, aspectRatio, zNear, zFar);
}

Mat4& MathsTransform::perspective(Mat4 & m, float fov, float aspectRatio, float zNear, float zFar)
{
	const float tanHalfFOV = (float)tan(fov / 2.0f);
	const float zRange = zFar - zNear;

	m[0][0] = 1.0f / (tanHalfFOV * aspectRatio);	m[1][0] = 0;					m[2][0] = 0;						m[3][0] = 0;
	m[0][1] = 0;									m[1][1] = 1.0f / tanHalfFOV;	m[2][1] = 0;						m[3][1] = 0;
	m[0][2] = 0;									m[1][2] = 0;					m[2][2] = (-zNear - zFar) / zRange;	m[3][2] = -(2.0f * zFar * zNear) / zRange;
	m[0][3] = 0;									m[1][3] = 0;					m[2][3] = -1.0f;					m[3][3] = 0;
	return m;
}

Mat4 MathsTransform::orthographic(float left, float right, float top, float bottom, float zNear, float zFar)
{
	Mat4 m;
	return orthographic(m, left, right, top, bottom, zNear, zFar);
}

Mat4& MathsTransform::orthographic(Mat4 & m, float left, float right, float top, float bottom, float zNear, float zFar)
{
	const float zRange = zFar - zNear;
	m[0][0] = 2.0f / (right - left);	m[1][0] = 0;						m[2][0] = 0;				m[3][0] = -(right + left) / (right - left);
	m[0][1] = 0;						m[1][1] = 2.0f / (top - bottom);	m[2][1] = 0;				m[3][1] = -(top + bottom) / (top - bottom);
	m[0][2] = 0;						m[1][2] = 0;						m[2][2] = -2.0f / zRange;	m[3][2] = -(zFar + zNear) / zRange;
	m[0][3] = 0;						m[1][3] = 0;						m[2][3] = 0;				m[3][3] = 1.0f;
	return m;
}

Mat4 MathsTransform::initCamera(const Vec3 & direction, const Vec3 & worldUp, const Vec3& position)
{
	Mat4 m;
	return initCamera(m, direction, worldUp, position);
}

Mat4& MathsTransform::initCamera(Mat4 & m, const Vec3 & direction, const Vec3 & worldUp, const Vec3& position)
{
	Vec3 right = worldUp;
	right = Maths::normalize(Maths::cross(direction, right));
	Vec3 up = Maths::normalize(Maths::cross(right, direction));
	return initCamera(m, right, up, direction, position);
}

Mat4 MathsTransform::initCamera(const Vec3 & right, const Vec3 & up, const Vec3 & forward, const Vec3& position)
{
	Mat4 m;
	return initCamera(m, right, up, forward, position);
}

Mat4& MathsTransform::initCamera(Mat4& m, const Vec3 & right, const Vec3 & up, const Vec3 & forward, const Vec3& position)
{
	m[0][0] = right.x;		m[1][0] = right.y;		m[2][0] = right.z;		m[3][0] = -Maths::dot(position, right);
	m[0][1] = up.x;			m[1][1] = up.y;			m[2][1] = up.z;			m[3][1] = -Maths::dot(position, up);
	m[0][2] = -forward.x;	m[1][2] = -forward.y;	m[2][2] = -forward.z;	m[3][2] = Maths::dot(position, forward);
	m[0][3] = 0;			m[1][3] = 0;			m[2][3] = 0;			m[3][3] = 1.0f;

	return m;
}

Vec3 MathsTransform::rotate(const Vec3 & v, float angle, const Vec3 & axis)
{
	Vec3 vPAxis = Maths::project<Vec3>(v, axis);
	Vec3 vOAxis = v - vPAxis;
	float x = (float)cos(angle);
	float y = (float)sin(angle);
	Vec3 vOr = Maths::length<Vec3>(vOAxis) * (x * Maths::normalize<Vec3>(vOAxis) + y * Maths::normalize<Vec3>(Maths::cross(axis, vOAxis)));
	return vOr + vPAxis;
}

Vec3 MathsTransform::rotate(const Vec3 & v, float x, float y, float z)
{
	return Vec3(rotationMat(x, y, z) * v);
}

Mat3 MathsTransform::rotate(const Mat3 & m, float x, float y, float z)
{
	return Mat3(rotationMat(x, y, z) * Mat4(m));
}

Mat3 MathsTransform::rotation(float angle)
{
	const float c = cos(angle);
	const float s = sin(angle);
	return Mat3(c, -s, 0.0f,
				s, c, 0.0f,
				0.0f, 0.0f, 1.0f);
}

Vec4 MathsTransform::rotate(const Vec4& v, float angle, const Vec3& axis)
{
	Vec3 result = rotate(Vec3(v), angle, axis);
	return Vec3(result, v.w);
}

Vec4 MathsTransform::rotate(const Vec4& v, float x, float y, float z)
{
	return rotationMat(x, y, z) * v;
}

Mat4 MathsTransform::rotate(const Mat4& m, float x, float y, float z)
{
	return rotationMat(x, y, z) * m;
}

Mat4 MathsTransform::rotationMat(float x, float y, float z)
{
	float sinX = (float)sin(x);
	float cosX = (float)cos(x);
	float sinY = (float)sin(y);
	float cosY = (float)cos(y);
	float sinZ = (float)sin(z);
	float cosZ = (float)cos(z);

	Mat4 m, mx, my, mz;
	mx[0][0] = 1;		mx[1][0] = 0;		mx[2][0] = 0;		mx[3][0] = 0;
	mx[0][1] = 0;		mx[1][1] = cosX;	mx[2][1] = -sinX;	mx[3][1] = 0;
	mx[0][2] = 0;		mx[1][2] = sinX;	mx[2][2] = cosX;	mx[3][2] = 0;
	mx[0][3] = 0;		mx[1][3] = 0;		mx[2][3] = 0;		mx[3][3] = 1;

	my[0][0] = cosY;	my[1][0] = 0;		my[2][0] = sinY;	my[3][0] = 0;
	my[0][1] = 0;		my[1][1] = 1;		my[2][1] = 0;		my[3][1] = 0;
	my[0][2] = -sinY;	my[1][2] = 0;		my[2][2] = cosY;	my[3][2] = 0;
	my[0][3] = 0;		my[1][3] = 0;		my[2][3] = 0;		my[3][3] = 1;

	mz[0][0] = cosZ;	mz[1][0] = -sinZ;	mz[2][0] = 0;		mz[3][0] = 0;
	mz[0][1] = sinZ;	mz[1][1] = cosZ;	mz[2][1] = 0;		mz[3][1] = 0;
	mz[0][2] = 0;		mz[1][2] = 0;		mz[2][2] = 1;		mz[3][2] = 0;
	mz[0][3] = 0;		mz[1][3] = 0;		mz[2][3] = 0;		mz[3][3] = 1;

	m = mz * my * mx;
	return m;
}

Mat4 MathsTransform::lookAt(const Vec3 & target, const Vec3& position)
{
	Mat4 m;
	return lookAt(m, target, position);
}

Mat4 & MathsTransform::lookAt(Mat4 & m, const Vec3 & target, const Vec3& position)
{
	return lookIn(m, Maths::normalize(target - position));
}

Mat4 MathsTransform::lookIn(const Vec3 & direction)
{
	Mat4 m;
	return lookIn(m, direction);
}

Mat4 & MathsTransform::lookIn(Mat4 & m, const Vec3 & direction)
{
	Vec3 right = Vec3(0.0f, 1.0f, 0.0f);
	right = Maths::normalize(Maths::cross(direction, right));
	Vec3 up = Maths::normalize(Maths::cross(right, direction));
	m[0][0] = right.x;		m[1][0] = up.x;		m[2][0] = direction.x;	m[3][0] = 0.0f;
	m[0][1] = right.y;		m[1][1] = up.y;		m[2][1] = direction.y;	m[3][1] = 0.0f;
	m[0][2] = right.z;		m[1][2] = up.z;		m[2][2] = direction.z;	m[3][2] = 0.0f;
	m[0][3] = 0;			m[1][3] = 0;		m[2][3] = 0;			m[3][3] = 1.0f;
	return m;
}

Mat4 MathsTransform::scale(float x, float y, float z)
{
	Mat4 m(1.0f);
	return scale(m, x, y, z);
}

Mat4 & MathsTransform::scale(Mat4 & m, float x, float y, float z)
{
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	return m;
}

Mat4 MathsTransform::translate(float x, float y, float z)
{
	Mat4 m(1.0f);
	return translate(m, x, y, z);
}

Mat4 & MathsTransform::translate(Mat4 & m, float x, float y, float z)
{
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	return m;
}

Mat4 MathsTransform::getTranslationMat(const Mat4 & m)
{
	return translate(m[3][0], m[3][1], m[3][2]);
}
