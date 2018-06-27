#ifndef MATRIX_2_H
#define MATRIX_2_H

#include "../Vectors/Vec2.h"

class Mat3;
class Mat4;
class Mat2
{
private:
	Vec2 col[2];

public:
	// Identity
	Mat2();
	// Fill diagonal with value.
	Mat2(float value);
	// Set each column.
	Mat2(const Vec2& col1, const Vec2& col2);
	// Set each element in each row.
	Mat2(float e11, float e12, float e21, float e22);
	Mat2(const Mat4& m);
	Mat2(const Mat3& m);
	Mat2(const Mat2& m);

	// Get a specific column. 
	Vec2& operator[](size_t i);
	const Vec2& operator[](size_t i) const;

	Vec2 getRow(size_t i) const;
	Vec2& getColumn(size_t i);
	const Vec2& getColumn(size_t i) const;

	Mat2& operator=(const Mat2& m);
	Mat2& operator*=(const Mat2& m);
	Mat2& operator*=(float scalar);

	Mat2 operator*(const Mat2& m) const;
	Vec2 operator*(const Vec2& v) const;
	Mat2 operator*(float scalar) const;

	int getSideLength() const;

private:
	Mat2& copy(const Mat2& m);
};

#endif
