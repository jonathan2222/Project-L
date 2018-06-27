#ifndef VECTOR_3_H
#define VECTOR_3_H

class Vec2;
class Vec4;
class Vec3
{
public:
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float s, t, p; };
	};

private:
	typedef float Vec3::* const vec[3];
	static const vec v;

public:
	// Set to zero.
	Vec3();
	// Fill it with value
	Vec3(float value); 
	Vec3(float x, float y);
	Vec3(float x, float y, float z);
	Vec3(const Vec3& v);
	Vec3(const Vec2& v, float z = 0.0f);
	Vec3(const Vec4& v);

	float& operator[](size_t i);
	const float& operator[](size_t i) const;

	bool operator==(const Vec3& v);
	bool operator!=(const Vec3& v);

	Vec3& operator=(const Vec3& v);

	Vec3& operator+=(const Vec3& v);
	Vec3& operator-=(const Vec3& v);
	Vec3& operator+=(float scalar);
	Vec3& operator-=(float scalar);
	Vec3& operator*=(float scalar);
	Vec3& operator/=(float scalar);

	Vec3 operator+(const Vec3& v) const;
	Vec3 operator-(const Vec3& v) const;
	Vec3 operator+(float scalar) const;
	Vec3 operator-(float scalar) const;
	Vec3 operator*(float scalar) const;
	Vec3 operator/(float scalar) const;

	Vec3& operator++();
	Vec3& operator--();
	Vec3 operator++(int a) const;
	Vec3 operator--(int a) const;
	Vec3 operator-() const;

	int length() const;
	Vec2 toVec2() const;
	Vec4 toVec4() const;
};

inline Vec3 operator+(float scalar, const Vec3& v) { return v + scalar; }
inline Vec3 operator-(float scalar, const Vec3& v) { return v - scalar; }
inline Vec3 operator*(float scalar, const Vec3& v) { return v * scalar; }

#endif
