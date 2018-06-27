#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <iomanip>

#include "../Maths/Maths.h"

namespace Utils
{
	static const std::string toHex(int value)
	{
		std::stringstream ss;
		ss << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << value;
		return ss.str();
	}

	static const std::string toString(float value)
	{
		return std::to_string(value);
	}

	static const std::string toString(int value)
	{
		return std::to_string(value);
	}

	static const std::string toString(unsigned int value)
	{
		return std::to_string(value);
	}

	static const std::string toString(const Vec2& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + ")";
	}

	static const std::string toString(const Vec3& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	}

	static const std::string toString(const Vec4& vec)
	{
		return "(" + std::to_string(vec.x) + "," + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ", " + std::to_string(vec.w) + ")";
	}

	static const std::string toString(const Mat2& mat)
	{
		std::string str = "";
		for (unsigned int y = 0; y < 2; y++)
			for (unsigned int x = 0; x < 2; x++)
			{
				bool isPositive = mat[x][y] >= 0.0f;
				std::string spacing = (isPositive && x != 0) ? " " : "";
				str += (x == 0 ? "| " : "") + spacing + std::to_string(mat[x][y]) + (x == 1 ? " |\n" : ", ");
			}
		return str;
	}

	static const std::string toString(const Mat3& mat)
	{
		std::string str = "";
		for (unsigned int y = 0; y < 3; y++)
			for (unsigned int x = 0; x < 3; x++)
			{
				bool isPositive = mat[x][y] >= 0.0f;
				std::string spacing = (isPositive && x != 0) ? " " : "";
				str += (x == 0 ? "| " : "") + spacing + std::to_string(mat[x][y]) + (x == 2 ? " |\n" : ", ");
			}
		return str;
	}

	static const std::string toString(const Mat4& mat)
	{
		std::string str = "";
		for (unsigned int y = 0; y < 4; y++)
			for (unsigned int x = 0; x < 4; x++)
			{
				bool isPositive = mat[x][y] >= 0.0f;
				std::string spacing = (isPositive && x != 0) ? " " : "";
				str += (x == 0 ? "| " : "") + spacing + std::to_string(mat[x][y]) + (x == 3 ? " |\n" : ", ");
			}
		return str;
	}
}

#endif
