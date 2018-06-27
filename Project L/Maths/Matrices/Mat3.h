#ifndef MATRIX_3_H
#define MATRIX_3_H

#include "../Vectors/Vec3.h"

class Mat2;
class Mat4;
class Mat3
{
private:
	Vec3 col[3];

public:
	// Identity
	Mat3();
	// Fill diagonal with value.
	Mat3(float value);
	// Set each column.
	Mat3(const Vec3& col1, const Vec3& col2, const Vec3& col3);
	// Set each element in each row.
	Mat3(float e11, float e12, float e13, float e21, float e22, float e23, float e31, float e32, float e33);
	Mat3(const Mat4& m);
	Mat3(const Mat3& m);
	Mat3(const Mat2& m);

	// Get a specific column. 
	Vec3& operator[](size_t i);
	const Vec3& operator[](size_t i) const;

	Vec3 getRow(size_t i) const;
	Vec3& getColumn(size_t i);
	const Vec3& getColumn(size_t i) const;

	Mat3& operator=(const Mat3& m);
	Mat3& operator*=(const Mat3& m);
	Mat3& operator*=(float scalar);

	Mat3 operator*(const Mat3& m) const;
	Vec3 operator*(const Vec3& v) const;
	Mat3 operator*(float scalar) const;

	int getSideLength() const;

private:
	Mat3& copy(const Mat3& m);
};

#endif
