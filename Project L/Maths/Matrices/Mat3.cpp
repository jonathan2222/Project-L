#include "Mat3.h"

#include "Mat2.h"
#include "Mat4.h"

Mat3::Mat3()
{
	this->col[0].x = 0.0f;	this->col[1].x = 0.0f;	this->col[2].x = 0.0f;
	this->col[0].y = 0.0f;	this->col[1].y = 0.0f;	this->col[2].y = 0.0f;
	this->col[0].z = 0.0f;	this->col[1].z = 0.0f;	this->col[2].z = 0.0f;
}

Mat3::Mat3(float value)
{
	this->col[0].x = value;	this->col[1].x = 0.0f;	this->col[2].x = 0.0f;
	this->col[0].y = 0.0f;	this->col[1].y = value;	this->col[2].y = 0.0f;
	this->col[0].z = 0.0f;	this->col[1].z = 0.0f;	this->col[2].z = value;
}

Mat3::Mat3(const Vec3 & col1, const Vec3 & col2, const Vec3 & col3)
{
	this->col[0] = col1;
	this->col[1] = col2;
	this->col[2] = col3;
}

Mat3::Mat3(float e11, float e12, float e13, float e21, float e22, float e23, float e31, float e32, float e33)
{
	this->col[0].x = e11;	this->col[1].x = e12;	this->col[2].x = e13;
	this->col[0].y = e21;	this->col[1].y = e22;	this->col[2].y = e23;
	this->col[0].z = e31;	this->col[1].z = e32;	this->col[2].z = e33;
}

Mat3::Mat3(const Mat4 & m)
{
	this->col[0].x = m[0].x;	this->col[1].x = m[1].x;	this->col[2].x = m[2].x;
	this->col[0].y = m[0].y;	this->col[1].y = m[1].y;	this->col[2].y = m[2].y;
	this->col[0].z = m[0].z;	this->col[1].z = m[1].z;	this->col[2].z = m[2].z;
}

Mat3::Mat3(const Mat3 & m)
{
	copy(m);
}

Mat3::Mat3(const Mat2 & m)
{
	this->col[0].x = m[0].x;	this->col[1].x = m[1].x;	this->col[2].x = 0.0f;
	this->col[0].y = m[0].y;	this->col[1].y = m[1].y;	this->col[2].y = 0.0f;
	this->col[0].z = 0.0f;		this->col[1].z = 0.0f;		this->col[2].z = 1.0f;
}

Vec3 & Mat3::operator[](size_t i)
{
	return getColumn(i);
}

const Vec3 & Mat3::operator[](size_t i) const
{
	return getColumn(i);
}

Vec3 Mat3::getRow(size_t i) const
{
	return Vec3(this->col[0][i], this->col[1][i], this->col[2][i]);
}

Vec3 & Mat3::getColumn(size_t i)
{
	return this->col[i];
}

const Vec3 & Mat3::getColumn(size_t i) const
{
	return this->col[i];
}

Mat3 & Mat3::operator=(const Mat3 & m)
{
	return copy(m);
}

Mat3 & Mat3::operator*=(const Mat3 & m)
{
	return copy(*this * m);
}

Mat3 & Mat3::operator*=(float scalar)
{
	for (size_t c = 0; c < 3; c++)
		for (size_t r = 0; r < 3; r++)
			this->col[c][r] *= scalar;
	return *this;
}

Mat3 Mat3::operator*(const Mat3 & m) const
{
	Mat3 ret;
	for (size_t r = 0; r < 3; r++)
		for (size_t c = 0; c < 3; c++)
			ret[c][r] = this->col[0][r] * m[c][0] + this->col[1][r] * m[c][1] + this->col[2][r] * m[c][2];
	return ret;
}

Vec3 Mat3::operator*(const Vec3 & v) const
{
	Vec3 ret;
	for (size_t r = 0; r < 3; r++)
		ret[r] = this->col[0][r] * v[0] + this->col[1][r] * v[1] + this->col[2][r] * v[2];
	return ret;
}

Mat3 Mat3::operator*(float scalar) const
{
	Mat3 m(*this);
	return m *= scalar;
}

int Mat3::getSideLength() const
{
	return 3;
}

Mat3 & Mat3::copy(const Mat3 & m)
{
	for (size_t c = 0; c < 3; c++)
		this->col[c] = m[c];
	return *this;
}
