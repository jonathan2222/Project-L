#ifndef MATRIX_4_H
#define MATRIX_4_H

#include "../Vectors/Vec4.h"

class Mat3;
class Mat2;
class Mat4
{
private:
	Vec4 col[4];

public:
	// Identity
	Mat4();
	// Fill diagonal with value.
	Mat4(float value); 
	// Set each column.
	Mat4(const Vec4& col1, const Vec4& col2, const Vec4& col3, const Vec4& col4);
	// Set each element in each row.
	Mat4(float e11, float e12, float e13, float e14, 
		float e21, float e22, float e23, float e24, 
		float e31, float e32, float e33, float e34, 
		float e41, float e42, float e43, float e44);
	Mat4(const Mat4& m);
	Mat4(const Mat3& m);
	Mat4(const Mat2& m);

	// Get a specific column. 
	Vec4& operator[](size_t i);
	const Vec4& operator[](size_t i) const;

	Vec4 getRow(size_t i) const;
	Vec4& getColumn(size_t i);
	const Vec4& getColumn(size_t i) const;

	Mat4& operator=(const Mat4& m);
	Mat4& operator*=(const Mat4& m);
	Mat4& operator*=(float scalar);

	Mat4 operator*(const Mat4& m) const;
	Vec4 operator*(const Vec4& v) const;
	Mat4 operator*(float scalar) const;

	int getSideLength() const;

private:
	Mat4& copy(const Mat4& m);
};

#endif
