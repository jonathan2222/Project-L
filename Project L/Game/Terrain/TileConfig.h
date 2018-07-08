#ifndef TILE_CONFIG_H
#define TILE_CONFIG_H

#define TILE_IMG_SIZE 8
#define TILE_MAP_IMG_SIZE 1024
#define MIN_UV(indexX, indexY) Vec2((float)(indexX*TILE_IMG_SIZE), (float)(indexY*TILE_IMG_SIZE))
#define MIN_UV_PATCH(indexX, indexY, isHori) Vec2((float)(indexX*TILE_IMG_SIZE + isHori*(TILE_IMG_SIZE/2)), (float)(indexY*TILE_IMG_SIZE + (1 - isHori)*(TILE_IMG_SIZE/2)))

#include <string>

struct TileConfig
{
	enum TILE_TYPE {
		TILE_EMPTY = 0,
		TILE_WIRE_FRAME = 1,
		TILE_SKY,

		TILE_DIRT,
		TILE_STONE,
		TILE_STONE_2,
		TILE_GRASS,
		TILE_STONE_GOLD,
		
		MAX_NUM_TYPES
	};

	enum TILE_MASK
	{
		MASK_EMPTY = 0,
		MASK_PATCH_FULL = 1,

		MASK_PATCH_TL,
		MASK_PATCH_TR,
		MASK_PATCH_BL,
		MASK_PATCH_BR,
		MASK_PATCH_T,
		MASK_PATCH_L,
		MASK_PATCH_R,
		MASK_PATCH_B,

		MASK_PATCH_FILLED_TL,
		MASK_PATCH_FILLED_TR,
		MASK_PATCH_FILLED_BL,
		MASK_PATCH_FILLED_BR,
		MASK_PATCH_FILLED_T,
		MASK_PATCH_FILLED_L,
		MASK_PATCH_FILLED_R,
		MASK_PATCH_FILLED_B,

		MASK_PATCH_SOLO,
		MASK_PATCH_SOLO_T,
		MASK_PATCH_SOLO_B,
		MASK_PATCH_SOLO_L,
		MASK_PATCH_SOLO_R,

		MASK_PATCH_TUBE_BLOCKED_V,
		MASK_PATCH_TUBE_BLOCKED_H,
		MASK_PATCH_TUBE_V,
		MASK_PATCH_TUBE_H,

		MAX_NUM_MASKS
	};
	
	static Vec2 getMinUvFromTileType(TILE_TYPE type)
	{
		switch (type)
		{
			case TILE_WIRE_FRAME:		return Vec2(MIN_UV(1, 0)); break;
			case TILE_SKY:				return Vec2(MIN_UV(0, 2)); break;

			case TILE_GRASS:			return Vec2(MIN_UV(0, 3)); break;
			case TILE_DIRT:				return Vec2(MIN_UV(0, 4)); break;
			case TILE_STONE:			return Vec2(MIN_UV(0, 5)); break;
			case TILE_STONE_2:			return Vec2(MIN_UV(0, 6)); break;
			case TILE_STONE_GOLD:		return Vec2(MIN_UV(0, 7)); break;

			case TILE_EMPTY:
			default:					return Vec2(MIN_UV(0, 0));
		}
	}

	static Vec2 getMinUvMaskFromTileMask(TILE_MASK type)
	{
		switch (type)
		{
			case MASK_PATCH_TL:				return Vec2(MIN_UV(1, 2)); break;
			case MASK_PATCH_TR:				return Vec2(MIN_UV(3, 2)); break;
			case MASK_PATCH_BL:				return Vec2(MIN_UV(1, 4)); break;
			case MASK_PATCH_BR:				return Vec2(MIN_UV(3, 4)); break;
			case MASK_PATCH_T:				return Vec2(MIN_UV(2, 2)); break;
			case MASK_PATCH_L:				return Vec2(MIN_UV(1, 3)); break;
			case MASK_PATCH_R:				return Vec2(MIN_UV(3, 3)); break;
			case MASK_PATCH_B:				return Vec2(MIN_UV(2, 4)); break;

			case MASK_PATCH_FILLED_TL:		return Vec2(MIN_UV(4, 2)); break;
			case MASK_PATCH_FILLED_TR:		return Vec2(MIN_UV(6, 2)); break;
			case MASK_PATCH_FILLED_BL:		return Vec2(MIN_UV(4, 4)); break;
			case MASK_PATCH_FILLED_BR:		return Vec2(MIN_UV(6, 4)); break;
			case MASK_PATCH_FILLED_T:		return Vec2(MIN_UV(5, 2)); break;
			case MASK_PATCH_FILLED_L:		return Vec2(MIN_UV(4, 3)); break;
			case MASK_PATCH_FILLED_R:		return Vec2(MIN_UV(6, 3)); break;
			case MASK_PATCH_FILLED_B:		return Vec2(MIN_UV(5, 4)); break;

			case MASK_PATCH_SOLO:			return Vec2(MIN_UV(0, 1)); break;
			case MASK_PATCH_SOLO_T:			return Vec2(MIN_UV(2, 0)); break;
			case MASK_PATCH_SOLO_B:			return Vec2(MIN_UV(3, 0)); break;
			case MASK_PATCH_SOLO_L:			return Vec2(MIN_UV(3, 1)); break;
			case MASK_PATCH_SOLO_R:			return Vec2(MIN_UV(2, 1)); break;

			case MASK_PATCH_TUBE_BLOCKED_V:	return Vec2(MIN_UV(4, 1)); break;
			case MASK_PATCH_TUBE_BLOCKED_H:	return Vec2(MIN_UV(5, 1)); break;
			case MASK_PATCH_TUBE_V:			return Vec2(MIN_UV(4, 0)); break;
			case MASK_PATCH_TUBE_H:			return Vec2(MIN_UV(5, 0)); break;

			case MASK_PATCH_FULL:			return Vec2(MIN_UV(1, 1)); break;
			case MASK_EMPTY:
			default:						return Vec2(MIN_UV(0, 0));
		}
	}

	static bool isMinUvMaskOfTypePatchBorder(Vec2 minUv)
	{
		return minUv == getMinUvMaskFromTileMask(MASK_PATCH_T) || minUv == getMinUvMaskFromTileMask(MASK_PATCH_TL) || minUv == getMinUvMaskFromTileMask(MASK_PATCH_TR) ||
			minUv == getMinUvMaskFromTileMask(MASK_PATCH_L) || minUv == getMinUvMaskFromTileMask(MASK_PATCH_R) || minUv == getMinUvMaskFromTileMask(MASK_PATCH_BL) ||
			minUv == getMinUvMaskFromTileMask(MASK_PATCH_BR) || minUv == getMinUvMaskFromTileMask(MASK_PATCH_B);
	}

#define MASK_TO_STR_CASE_RETURN(mask) case mask: return #mask; break;
#define MASK_TO_STR(mask) #mask;
	static std::string getStrFromMask(TILE_MASK mask)
	{
		switch (mask)
		{
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_TL)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_TR)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_BL)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_BR)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_T)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_L)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_R)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_B)

			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_TL)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_TR)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_BL)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_BR)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_T)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_L)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_R)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FILLED_B)

			MASK_TO_STR_CASE_RETURN(MASK_PATCH_SOLO)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_SOLO_T)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_SOLO_B)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_SOLO_L)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_SOLO_R)

			MASK_TO_STR_CASE_RETURN(MASK_PATCH_TUBE_BLOCKED_V)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_TUBE_BLOCKED_H)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_TUBE_V)
			MASK_TO_STR_CASE_RETURN(MASK_PATCH_TUBE_H)

			MASK_TO_STR_CASE_RETURN(MASK_PATCH_FULL)
			MASK_TO_STR_CASE_RETURN(MASK_EMPTY)
			default: return "UNKNOWN";
		}
	}

	static std::string getStrFromMinUvMask(Vec2 mask)
	{
		for (unsigned int i = 0; i < TILE_MASK::MAX_NUM_MASKS; i++)
			if (getMinUvMaskFromTileMask((TILE_MASK)i) == mask)
				return getStrFromMask((TILE_MASK)i);
		return getStrFromMask(TILE_MASK::MAX_NUM_MASKS);
	}

	static std::string getStrFromType(TILE_TYPE mask)
	{
		switch (mask)
		{
			MASK_TO_STR_CASE_RETURN(TILE_WIRE_FRAME)
			MASK_TO_STR_CASE_RETURN(TILE_SKY)
			MASK_TO_STR_CASE_RETURN(TILE_GRASS)
			MASK_TO_STR_CASE_RETURN(TILE_DIRT)
			MASK_TO_STR_CASE_RETURN(TILE_STONE)
			MASK_TO_STR_CASE_RETURN(TILE_STONE_2)
			MASK_TO_STR_CASE_RETURN(TILE_STONE_GOLD)
			MASK_TO_STR_CASE_RETURN(TILE_EMPTY)
			default: return "UNKNOWN";
		}
	}

	static std::string getStrFromMinUv(Vec2 type)
	{
		for (unsigned int i = 0; i < TILE_TYPE::MAX_NUM_TYPES; i++)
			if (getMinUvFromTileType((TILE_TYPE)i) == type)
				return getStrFromType((TILE_TYPE)i);
		return getStrFromType(TILE_TYPE::MAX_NUM_TYPES);
	}
};

#endif
