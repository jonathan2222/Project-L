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

	// Author @patriciogv - 2015
	// http://patriciogonzalezvivo.com
	float random(const Vec2& x)
	{
		return Maths::fract(sin(Maths::dot(x, Vec2(12.9898f, 78.233f)))*43758.5453123f);
	}

	// Author @patriciogv - 2015
	// http://patriciogonzalezvivo.com
	float noise(const Vec2& x)
	{
		Vec2 i(floor(x.x), floor(x.y));
		Vec2 f(Maths::fract(x.x), Maths::fract(x.y));

		// Four corners in 2D of a tile
		float a = random(i);
		float b = random(i + Vec2(1.0f, 0.0f));
		float c = random(i + Vec2(0.0f, 1.0f));
		float d = random(i + Vec2(1.0f, 1.0f));

		Vec2 f2(3.0f - 2.0f * f.x, 3.0f - 2.0f * f.y);
		Vec2 u(f.x*f.x*f2.x, f.y*f.y*f2.y);

		return Maths::lerp(a, b, u.x) +
			(c - a)* u.y * (1.0f - u.x) +
			(d - b) * u.x * u.y;
	}

	float fbm(Vec2 x, unsigned int OCTAVES = 6) {
		// Initial values
		float value = 0.0;
		float amplitude = .5;
		float frequency = 0.;
		
		// Loop of octaves
		for (unsigned int i = 0; i < OCTAVES; i++) {
			value += amplitude * noise(x);
			x *= 2.0f;
			amplitude *= .5;
		}
		return value;
	}

	/**
	* Returns a random value between -1.0 and 1.0.
	* Will return the same value if the same seed is used.
	* @param seed
	* @return a random number between -1.0 and 1.0.
	*/
	float noise(int seed) {
		seed = (seed << 13) ^ seed;
		return 1.0f - (((seed * (seed * seed * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}

	/**
	* Returns a random value between -1.0 and 1.0.
	* Will return the same value if the same seed(x and y) is used.
	* @param x the x seed.
	* @param y the y seed.
	* @return a random number between -1.0 and 1.0.
	*/
	float noise(int x, int y) {
		int n = x + y * 57;
		n = (n << 13) ^ n;
		return 1.0f - (((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}

	/**
	* Smooth the x value with the use of the neighboring values and use the noise function to get a random value.
	* @param x
	* @return a value between -1.0 and 1.0.
	*/
	float smoothedNoise(float x) {
		return noise((int)x) / 2.f + noise((int)x - 1) / 4.f + noise((int)x + 1) / 2.f;
	}

	/**
	* Smooth the point with the use of the neighboring values and use the noise function to get a random value.
	* @param x
	* @param y
	* @return a value between -1.0 and 1.0.
	*/
	float smoothNoise(float x, float y) {
		int x1 = (int)x;
		int y1 = (int)y;
		//Neighbor values
		int x2 = x1 - 1;
		int y2 = y1 - 1;
		int x3 = x1 + 1;
		int y3 = y1 + 1;
		float corners = (noise(x2, y2) + noise(x3, y2) + noise(x2, y3) + noise(x3, y3)) / 16.f;
		float sides = (noise(x2, y1) + noise(x3, y1) + noise(x1, y2) + noise(x1, y3)) / 8.f;
		float center = noise(x1, y1) / 4.f;
		return corners + sides + center;
	}

	// Interpolation function flags, Only one interpolation function can be used at a time.
	const int LINEAR_INTERPOLATION = 0;
	const int COSINE_INTERPOLATION = 1;
	const int CUBIC_HERMITE_3ED_ORDER_INTERPOLATION = 2;
	const int CUBIC_HERMITE_5TH_ORDER_INTERPOLATION = 3;
	const int CUBIC_HERMITE_7TH_ORDER_INTERPOLATION = 4;

	/**
	* A interpolation function to interpolate between two values. Specify a curtain interpolation function to be used.
	* @param a
	* @param b
	* @param x
	* @param interpolationFunction
	* @return
	*/
	float interpolate(float a, float b, float x, int interpolationFunction) {
		switch (interpolationFunction) {
			case LINEAR_INTERPOLATION:
				return Maths::lerp(a, b, x);
			case COSINE_INTERPOLATION:
				return Maths::cosineLerp(a, b, x);
			case CUBIC_HERMITE_3ED_ORDER_INTERPOLATION:
				return Maths::cubicHermiteInterpolation3rdOrder(a, b, x);
			case CUBIC_HERMITE_5TH_ORDER_INTERPOLATION:
				return Maths::cubicHermiteInterpolation5thOrder(a, b, x);
			case CUBIC_HERMITE_7TH_ORDER_INTERPOLATION:
				return Maths::cubicHermiteInterpolation7thOrder(a, b, x);
			default:
				return Maths::lerp(a, b, x);
		}
	}

	/**
	* Interpolates the x value with the use of a noise function and the specified interpolation function.
	* Example: If interpolationFunction is Maths.LINEAR_INTERPOLATION then this will use a linear interpolation to interpolate the value between this and the value+1.
	* @param x
	* @param interpolationFunction
	* @return a value between 0 and 1.
	*/
	float interpolateNoise(float x, int interpolationFunction) {
		float intX = floor(x);
		float fract = Maths::fract(x);
		float v1 = (smoothedNoise(intX) + 1.f) / 2.f;
		float v2 = (smoothedNoise(intX + 1) + 1.f) / 2.f;
		return interpolate(v1, v2, fract, interpolationFunction);
	}

	/**
	* Interpolates the point with the use of a noise function and the specified interpolation function.
	* Example: If interpolationFunction is Maths.LINEAR_INTERPOLATION then this will use a linear interpolation to interpolate the point between this and the point+1.
	* @param x
	* @param y
	* @param interpolationFunction
	* @return a value between 0 and 1.
	*/
	float interpolateNoise(float x, float y, int interpolationFunction) {
		float intX = floor(x);
		float fractX = Maths::fract(x);
		float intY = floor(y);
		float fractY = Maths::fract(y);
		float v1 = (smoothNoise(intX, intY) + .1f) / 2.f;
		float v2 = (smoothNoise(intX + 1.f, intY) + 1.f) / 2.f;
		float v3 = (smoothNoise(intX, intY + 1.f) + 1.f) / 2.f;
		float v4 = (smoothNoise(intX + 1.f, intY + 1.f) + 1.f) / 2.f;
		float i1, i2;
		i1 = interpolate(v1, v2, fractX, interpolationFunction);
		i2 = interpolate(v3, v4, fractX, interpolationFunction);
		return interpolate(i1, i2, fractY, interpolationFunction);
	}

	/**
	* Apply turbulence to a interpolated noise function.
	* @param x
	* @param y
	* @param size the zoom size. If 1 nothing happens and it will be like a perlin noise function.
	* @param interpolationFunction is the type of interpolation to be used.
	* @return a value between 0 and 1.
	*/
	float turbulence(float x, float y, float size, int interpolationFunction) {
		float value = 0.0f, initailSize = size;
		while (size >= 1) {
			value += interpolateNoise(x / size, y / size, interpolationFunction)*size;
			size /= 2.0f;
		}
		return value / (initailSize * 2);
	}

	/**
	* Apply turbulence to a interpolated noise function.
	* @param x
	* @param size the zoom size. If 1 nothing happens and it will be like a perlin noise function.
	* @param interpolationFunction is the type of interpolation to be used.
	* @return a value between 0 and 1.
	*/
	float turbulence(float x, float size, int interpolationFunction) {
		float value = 0.0f, initailSize = size;
		while (size >= 1) {
			value += interpolateNoise(x / size, interpolationFunction)*size;
			size /= 2.0f;
		}
		return value / (initailSize * 2);
	}

	/**
	* Calculate Perlin Noise on the x value with the specified persistence.
	* @param x
	* @param presistence is the amplitude of each frequency.
	* @param lacuarity is the difference in frequency for each octave.(2 is the normal value for this)
	* @param numberOfOctaves is the number of octaves the Perlin Nose should use or you can see it as the number of different noise layers added.
	* @param interpolationFunction
	* @return a value between 0 and 1.
	*/
	float perlinNoise(float x, float persistence, float lacuarity, int numberOfOctaves, int interpolationFunction) {
		float total = 0;
		float frequency = 1;
		float amplitude = 1;
		float maxValue = 0;
		for (int i = 0; i < numberOfOctaves; i++) {
			total += interpolateNoise(x*frequency, interpolationFunction)*amplitude;
			maxValue += amplitude;
			frequency *= lacuarity;
			amplitude *= persistence;
		}
		return total / maxValue;
	}

	/**
	* Calculate Perlin Noise on the x value with the specified persistence.
	* @param x
	* @param presistence is the amplitude of each frequency.
	* @param lacuarity is the difference in frequency for each octave.(2 is the normal value for this)
	* @param numberOfOctaves is the number of octaves the Perlin Nose should use or you can see it as the number of different noise layers added.
	* @param interpolationFunction
	* @return a value between 0 and 1.
	*/
	float perlinNoiseUnNormalized(float x, float persistence, float lacuarity, int numberOfOctaves, int interpolationFunction) {
		float total = 0;
		float frequency = 1;
		float amplitude = 1;
		for (int i = 0; i < numberOfOctaves; i++) {
			total += interpolateNoise(x*frequency, interpolationFunction)*amplitude;
			frequency *= lacuarity;
			amplitude *= persistence;
		}
		return total;
	}

	/**
	* Calculate Perlin Noise on the point with the specified persistence.
	* @param x
	* @param y
	* @param presistence is the difference in amplitude for each octave.
	* @param lacuarity is the difference in frequency for each octave.(2 is the normal value for this)
	* @param numberOfOctaves is the number of octaves the Perlin Noise should use or you can see it as the number of different noise layers added.
	* @param interpolationFunction
	* @return a value between 0 and 1.
	*/
	float perlinNoise(float x, float y, float persistence, float lacuarity, int numberOfOctaves, int interpolationFunction) {
		float total = 0;
		float frequency = 1;
		float amplitude = 1;
		float maxValue = 0;
		for (int i = 0; i < numberOfOctaves; i++) {
			total += interpolateNoise(x*frequency, y*frequency, interpolationFunction)*amplitude;
			maxValue += amplitude;
			amplitude *= persistence;
			frequency *= lacuarity;
		}
		return total / maxValue;
	}

	/**
	* Calculate Perlin Noise on the point with the specified persistence.
	* @param x
	* @param y
	* @param presistence is the difference in amplitude for each octave.
	* @param lacuarity is the difference in frequency for each octave.(2 is the normal value for this)
	* @param numberOfOctaves is the number of octaves the Perlin Nose should use or you can see it as the number of different noise layers added.
	* @param interpolationFunction
	* @return a value between 0 and 1.
	*/
	float perlinNoiseUnNormalized(float x, float y, float persistence, float lacuarity, int numberOfOctaves, int interpolationFunction) {
		float total = 0;
		float frequency = 1;
		float amplitude = 1;
		for (int i = 0; i < numberOfOctaves; i++) {
			total += interpolateNoise(x*frequency, y*frequency, interpolationFunction)*amplitude;
			amplitude *= persistence;
			frequency *= lacuarity;
		}
		return total;
	}
	/*
	float[][] generatePerlinNoiseMap(int width, int height, float scale, float persistance, float lacuarity, int octaves, int interpolationFunction) {
		float[][] noiseMap = new float[width][height];
		float maxValue = Float.MIN_VALUE;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < width; y++) {
				float noise = perlinNoiseUnNormalized(x / scale, y / scale, persistance, lacuarity, octaves, interpolationFunction);
				maxValue = Math.max(maxValue, noise);
				noiseMap[x][y] = noise;
			}
		}
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < width; y++) {
				noiseMap[x][y] /= maxValue;
			}
		}
		return noiseMap;
	}*/
}

#endif
