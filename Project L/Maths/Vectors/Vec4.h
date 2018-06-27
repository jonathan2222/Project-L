#ifndef VECTOR_4F_H
#define VECTOR_4F_H

class Vec2;
class Vec3;
class Vec4
{
public:
	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		struct { float s, t, p, q; };
	};
	
private:
	typedef float Vec4::* const vec[4];
	static const vec v;

public:
	// Set to zero.
	Vec4();
	// Fill it with value.
	Vec4(float value); 
	Vec4(float x, float y);
	Vec4(float x, float y, float z);
	Vec4(float x, float y, float z, float w);
	Vec4(const Vec4& v);
	Vec4(const Vec3& v, float w = 0.0f);
	Vec4(const Vec2& v, float z = 0.0f, float w = 0.0f);

	float& operator[](size_t i);
	const float& operator[](size_t i) const;

	bool operator==(const Vec4& v);
	bool operator!=(const Vec4& v);

	Vec4& operator=(const Vec4& v);

	Vec4& operator+=(const Vec4& v);
	Vec4& operator-=(const Vec4& v);
	Vec4& operator+=(float scalar);
	Vec4& operator-=(float scalar);
	Vec4& operator*=(float scalar);
	Vec4& operator/=(float scalar);

	Vec4 operator+(const Vec4& v) const;
	Vec4 operator-(const Vec4& v) const;
	Vec4 operator+(float scalar) const;
	Vec4 operator-(float scalar) const;
	Vec4 operator*(float scalar) const;
	Vec4 operator/(float scalar) const;

	Vec4& operator++();
	Vec4& operator--();
	Vec4 operator++(int a) const;
	Vec4 operator--(int a) const;
	Vec4 operator-() const;

	int length() const;
	Vec2 toVec2() const;
	Vec3 toVec3() const;
};

inline Vec4 operator+(float scalar, const Vec4& v) { return v + scalar; }
inline Vec4 operator-(float scalar, const Vec4& v) { return v - scalar; }
inline Vec4 operator*(float scalar, const Vec4& v) { return v * scalar; }

#endif
