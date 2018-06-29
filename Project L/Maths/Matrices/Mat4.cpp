#include "Mat4.h"

#include "Mat3.h"
#include "Mat2.h"

Mat4::Mat4()
{
	this->col[0].x = 1.0f;	this->col[1].x = 0.0f;	this->col[2].x = 0.0f;	this->col[3].x = 0.0f;
	this->col[0].y = 0.0f;	this->col[1].y = 1.0f;	this->col[2].y = 0.0f;	this->col[3].y = 0.0f;
	this->col[0].z = 0.0f;	this->col[1].z = 0.0f;	this->col[2].z = 1.0f;	this->col[3].z = 0.0f;
	this->col[0].w = 0.0f;	this->col[1].w = 0.0f;	this->col[2].w = 0.0f;	this->col[3].w = 1.0f;
}

Mat4::Mat4(float value)
{
	this->col[0].x = value;	this->col[1].x = 0.0f;	this->col[2].x = 0.0f;	this->col[3].x = 0.0f;
	this->col[0].y = 0.0f;	this->col[1].y = value;	this->col[2].y = 0.0f;	this->col[3].y = 0.0f;
	this->col[0].z = 0.0f;	this->col[1].z = 0.0f;	this->col[2].z = value;	this->col[3].z = 0.0f;
	this->col[0].w = 0.0f;	this->col[1].w = 0.0f;	this->col[2].w = 0.0f;	this->col[3].w = value;
}

Mat4::Mat4(const Vec4 & col1, const Vec4 & col2, const Vec4 & col3, const Vec4 & col4)
{
	this->col[0] = col1;
	this->col[1] = col2;
	this->col[2] = col3;
	this->col[3] = col4;
}

Mat4::Mat4(float e11, float e12, float e13, float e14, 
				float e21, float e22, float e23, float e24, 
				float e31, float e32, float e33, float e34, 
				float e41, float e42, float e43, float e44)
{
	this->col[0].x = e11;	this->col[1].x = e12;	this->col[2].x = e13;	this->col[3].x = e14;
	this->col[0].y = e21;	this->col[1].y = e22;	this->col[2].y = e23;	this->col[3].y = e24;
	this->col[0].z = e31;	this->col[1].z = e32;	this->col[2].z = e33;	this->col[3].z = e34;
	this->col[0].w = e41;	this->col[1].w = e42;	this->col[2].w = e43;	this->col[3].w = e44;
}

Mat4::Mat4(const Mat4 & m)
{
	copy(m);
}

Mat4::Mat4(const Mat3 & m)
{
	this->col[0].x = m[0].x;	this->col[1].x = m[1].x;	this->col[2].x = m[2].x;	this->col[3].x = 0.0f;
	this->col[0].y = m[0].y;	this->col[1].y = m[1].y;	this->col[2].y = m[2].y;	this->col[3].y = 0.0f;
	this->col[0].z = m[0].z;	this->col[1].z = m[1].z;	this->col[2].z = m[2].z;	this->col[3].z = 0.0f;
	this->col[0].w = 0.0f;		this->col[1].w = 0.0f;		this->col[2].w = 0.0f;		this->col[3].w = 1.0f;
}

Mat4::Mat4(const Mat2 & m)
{
	this->col[0].x = m[0].x;	this->col[1].x = m[1].x;	this->col[2].x = 0.0f;		this->col[3].x = 0.0f;
	this->col[0].y = m[0].y;	this->col[1].y = m[1].y;	this->col[2].y = 0.0f;		this->col[3].y = 0.0f;
	this->col[0].z = 0.0f;		this->col[1].z = 0.0f;		this->col[2].z = 1.0f;		this->col[3].z = 0.0f;
	this->col[0].w = 0.0f;		this->col[1].w = 0.0f;		this->col[2].w = 0.0f;		this->col[3].w = 1.0f;
}

Vec4 & Mat4::operator[](size_t i)
{
	return getColumn(i);
}

const Vec4 & Mat4::operator[](size_t i) const
{
	return getColumn(i);
}

Vec4 Mat4::getRow(size_t i) const
{
	return Vec4(this->col[0][i], this->col[1][i], this->col[2][i], this->col[3][i]);
}

Vec4 & Mat4::getColumn(size_t i)
{
	return this->col[i];
}

const Vec4 & Mat4::getColumn(size_t i) const
{
	return this->col[i];
}

Mat4 & Mat4::operator=(const Mat4 & m)
{
	return copy(m);
}

Mat4 & Mat4::operator*=(const Mat4 & m)
{
	return copy(*this * m);
}

Mat4 & Mat4::operator*=(float scalar)
{
	for (size_t c = 0; c < 4; c++)
		for (size_t r = 0; r < 4; r++)
			this->col[c][r] *= scalar;
	return *this;
}

Mat4 Mat4::operator*(const Mat4 & m) const
{
	Mat4 ret;
	for (size_t r = 0; r < 4; r++)
		for (size_t c = 0; c < 4; c++)
			ret[c][r] = this->col[0][r] * m[c][0] + this->col[1][r] * m[c][1] + this->col[2][r] * m[c][2] + this->col[3][r] * m[c][3];
	return ret;
}

Vec4 Mat4::operator*(const Vec4 & v) const
{
	Vec4 ret;
	for (size_t r = 0; r < 4; r++)
		ret[r] = this->col[0][r] * v[0] + this->col[1][r] * v[1] + this->col[2][r] * v[2] + this->col[3][r] * v[3];
	return ret;
}

Mat4 Mat4::operator*(float scalar) const
{
	Mat4 m(*this);
	return m *= scalar;
}

int Mat4::getSideLength() const
{
	return 4;
}

Mat4 & Mat4::copy(const Mat4 & m)
{
	for (size_t c = 0; c < 4; c++)
		this->col[c] = m[c];
	return *this;
}
