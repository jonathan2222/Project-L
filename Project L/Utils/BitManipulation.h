#ifndef BIT_MANIPULATION_H
#define BIT_MANIPULATION_H

namespace Utils
{
	template<typename T>
	static const void setBit(T& mask, unsigned int bit)
	{
		mask |= (T)1 << (T)bit;
	}

	template<typename T>
	static const void clearBit(T& mask, unsigned int bit)
	{
		mask &= ~((T)1 << (T)bit);
	}

	template<typename T>
	static const void toggleBit(T& mask, unsigned int bit)
	{
		mask ^= (T)1 << (T)bit;
	}

	template<typename T>
	static const bool isBitSet(T& mask, unsigned int bit)
	{
		return (mask >> (T)bit) & (T)1;
	}
}

#endif
