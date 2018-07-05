#include "Vec4.h"
#include "Vec3.h"
#include "Vec2.h"

const Vec4::vec Vec4::v = { &Vec4::x, &Vec4::y, &Vec4::z, &Vec4::w };

Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

Vec4::Vec4(float value) : x(value), y(value), z(value), w(value)
{
}

Vec4::Vec4(float x, float y) : x(x), y(y), z(0.0f), w(0.0f)
{
}

Vec4::Vec4(float x, float y, float z) : x(x), y(y), z(z), w(0.0f)
{
}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vec4::Vec4(const Vec4 & v) : x(v.x), y(v.y), z(v.z), w(v.w)
{
}

Vec4::Vec4(const Vec3 & v, float w) : x(v.x), y(v.y), z(v.z), w(w)
{
}

Vec4::Vec4(const Vec2 & v, float z, float w) : x(v.x), y(v.y), z(z), w(w)
{
}

Vec4::Vec4(const Vec2 & v, const Vec2 & v2) : x(v.x), y(v.y), z(v2.x), w(v2.y)
{
}

float & Vec4::operator[](size_t i)
{
	return this->*v[i];
}

const float & Vec4::operator[](size_t i) const
{
	return this->*v[i];
}

bool Vec4::operator==(const Vec4 & v)
{
	return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
}

bool Vec4::operator!=(const Vec4 & v)
{
	return !(*this == v);
}

Vec4 & Vec4::operator=(const Vec4 & v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
	return *this;
}

Vec4 & Vec4::operator+=(const Vec4 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

Vec4 & Vec4::operator-=(const Vec4 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

Vec4 & Vec4::operator+=(float scalar)
{
	this->x += scalar;
	this->y += scalar;
	this->z += scalar;
	this->w += scalar;
	return *this;
}

Vec4 & Vec4::operator-=(float scalar)
{
	this->x -= scalar;
	this->y -= scalar;
	this->z -= scalar;
	this->w -= scalar;
	return *this;
}

Vec4 & Vec4::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	this->w *= scalar;
	return *this;
}

Vec4 & Vec4::operator/=(float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
	this->w /= scalar;
	return *this;
}

Vec4 Vec4::operator+(const Vec4 & v) const
{
	return Vec4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
}

Vec4 Vec4::operator-(const Vec4 & v) const
{
	return Vec4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
}

Vec4 Vec4::operator+(float scalar) const
{
	return Vec4(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar);
}

Vec4 Vec4::operator-(float scalar) const
{
	return Vec4(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar);
}

Vec4 Vec4::operator*(float scalar) const
{
	return Vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

Vec4 Vec4::operator/(float scalar) const
{
	return Vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
}

Vec4 & Vec4::operator++()
{
	this->x++;
	this->y++;
	this->z++;
	this->w++;
	return *this;
}

Vec4 & Vec4::operator--()
{
	this->x--;
	this->y--;
	this->z--;
	this->w--;
	return *this;
}

Vec4 Vec4::operator++(int a) const
{
	return Vec4(this->x + a, this->y + a, this->z + a, this->w + a);
}

Vec4 Vec4::operator--(int a) const
{
	return Vec4(this->x - a, this->y - a, this->z - a, this->w - a);
}

Vec4 Vec4::operator-() const
{
	return Vec4(-this->x, -this->y, -this->z, -this->w);
}

int Vec4::length() const
{
	return 4;
}

Vec2 Vec4::toVec2() const
{
	return Vec2(*this);
}

Vec3 Vec4::toVec3() const
{
	return Vec3(*this);
}
