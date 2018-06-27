#include "Vec3.h"

#include "Vec2.h"
#include "Vec4.h"

const Vec3::vec Vec3::v = { &Vec3::x, &Vec3::y, &Vec3::z };

Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(float value) : x(value), y(value), z(value)
{
}

Vec3::Vec3(float x, float y) : x(x), y(y), z(0.0f)
{

}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z)
{

}

Vec3::Vec3(const Vec3 & v) : x(v.x), y(v.y), z(v.z)
{
}

Vec3::Vec3(const Vec2 & v, float z) : x(v.x), y(v.y), z(z)
{
}

Vec3::Vec3(const Vec4 & v) : x(v.x), y(v.y), z(v.z)
{
}

float & Vec3::operator[](size_t i)
{
	return this->*v[i];
}

const float & Vec3::operator[](size_t i) const
{
	return this->*v[i];
}

bool Vec3::operator==(const Vec3 & v)
{
	return this->x == v.x && this->y == v.y && this->z == v.z;
}

bool Vec3::operator!=(const Vec3 & v)
{
	return !(*this == v);
}

Vec3 & Vec3::operator=(const Vec3 & v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

Vec3 & Vec3::operator+=(const Vec3 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vec3 & Vec3::operator-=(const Vec3 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vec3 & Vec3::operator+=(float scalar)
{
	this->x += scalar;
	this->y += scalar;
	this->z += scalar;
	return *this;
}

Vec3 & Vec3::operator-=(float scalar)
{
	this->x -= scalar;
	this->y -= scalar;
	this->z -= scalar;
	return *this;
}

Vec3 & Vec3::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

Vec3 & Vec3::operator/=(float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
	return *this;
}

Vec3 Vec3::operator+(const Vec3 & v) const
{
	return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3 Vec3::operator-(const Vec3 & v) const
{
	return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vec3 Vec3::operator+(float scalar) const
{
	return Vec3(this->x + scalar, this->y + scalar, this->z + scalar);
}

Vec3 Vec3::operator-(float scalar) const
{
	return Vec3(this->x - scalar, this->y - scalar, this->z - scalar);
}

Vec3 Vec3::operator*(float scalar) const
{
	return Vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vec3 Vec3::operator/(float scalar) const
{
	return Vec3(this->x / scalar, this->y / scalar, this->z / scalar);
}

Vec3 & Vec3::operator++()
{
	this->x++;
	this->y++;
	this->z++;
	return *this;
}

Vec3 & Vec3::operator--()
{
	this->x--;
	this->y--;
	this->z--;
	return *this;
}

Vec3 Vec3::operator++(int a) const
{
	return Vec3(this->x + a, this->y + a, this->z + a);
}

Vec3 Vec3::operator--(int a) const
{
	return Vec3(this->x - a, this->y - a, this->z - a);
}

Vec3 Vec3::operator-() const
{
	return Vec3(-this->x, -this->y, -this->z);
}

int Vec3::length() const
{
	return 3;
}

Vec2 Vec3::toVec2() const
{
	return Vec2(*this);
}

Vec4 Vec3::toVec4() const
{
	return Vec4(*this);
}
