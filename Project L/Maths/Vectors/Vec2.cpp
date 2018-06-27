#include "Vec2.h"

#include "Vec3.h"
#include "Vec4.h"

const Vec2::vec Vec2::v = { &Vec2::x, &Vec2::y };

Vec2::Vec2() : x(0.0f), y(0.0f)
{
}

Vec2::Vec2(float value) : x(value), y(value)
{
}

Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

Vec2::Vec2(const Vec2 & v) : x(v.x), y(v.y)
{
}

Vec2::Vec2(const Vec3 & v) : x(v.x), y(v.y)
{
}

Vec2::Vec2(const Vec4 & v) : x(v.x), y(v.y)
{
}

float & Vec2::operator[](size_t i)
{
	return this->*v[i];
}

const float & Vec2::operator[](size_t i) const
{
	return this->*v[i];
}

bool Vec2::operator==(const Vec2 & v)
{
	return this->x == v.x && this->y == v.y;
}

bool Vec2::operator!=(const Vec2 & v)
{
	return !(*this == v);
}

Vec2 & Vec2::operator=(const Vec2 & v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}

Vec2 & Vec2::operator+=(const Vec2 & v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vec2 & Vec2::operator-=(const Vec2 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Vec2 & Vec2::operator+=(float scalar)
{
	this->x += scalar;
	this->y += scalar;
	return *this;
}

Vec2 & Vec2::operator-=(float scalar)
{
	this->x -= scalar;
	this->y -= scalar;
	return *this;
}

Vec2 &Vec2::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	return *this;
}

Vec2 & Vec2::operator/=(float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	return *this;
}

Vec2 Vec2::operator+(const Vec2 & v) const
{
	return Vec2(this->x + v.x, this->y + v.y);
}

Vec2 Vec2::operator-(const Vec2 & v) const
{
	return Vec2(this->x - v.x, this->y - v.y);
}

Vec2 Vec2::operator+(float scalar) const
{
	return Vec2(this->x + scalar, this->y + scalar);
}

Vec2 Vec2::operator-(float scalar) const
{
	return Vec2(this->x - scalar, this->y - scalar);
}

Vec2 Vec2::operator*(float scalar) const
{
	return Vec2(this->x * scalar, this->y * scalar);
}

Vec2 Vec2::operator/(float scalar) const
{
	return Vec2(this->x / scalar, this->y / scalar);
}

Vec2 & Vec2::operator++()
{
	this->x++;
	this->y++;
	return *this;
}

Vec2 & Vec2::operator--()
{
	this->x--;
	this->y--;
	return *this;
}

Vec2 Vec2::operator++(int a) const
{
	return Vec2(this->x + a, this->y +a);
}

Vec2 Vec2::operator--(int a) const
{
	return Vec2(this->x - a, this->y - a);
}

Vec2 Vec2::operator-() const
{
	return Vec2(-this->x, -this->y);
}

int Vec2::length() const
{
	return 2;
}

Vec3 Vec2::toVec3() const
{
	return Vec3(*this);
}

Vec4 Vec2::toVec4() const
{
	return Vec4(*this);
}
