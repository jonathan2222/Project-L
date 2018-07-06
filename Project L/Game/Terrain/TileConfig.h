#ifndef TILE_CONFIG_H
#define TILE_CONFIG_H

#define TILE_IMG_SIZE 8
#define TILE_MAP_IMG_SIZE 1024
#define MIN_UV(indexX, indexY) Vec2((float)(indexX*TILE_IMG_SIZE), (float)(indexY*TILE_IMG_SIZE))
#define MIN_UV_PATCH(indexX, indexY, isHori) Vec2((float)(indexX*TILE_IMG_SIZE + isHori*(TILE_IMG_SIZE/2)), (float)(indexY*TILE_IMG_SIZE + (1 - isHori)*(TILE_IMG_SIZE/2)))

struct TileConfig
{
	enum TILE_TYPE {
		EMPTY = 0,

		DIRT = 1,
		STONE,
		GRASS,

		WIRE_FRAME,
		SKY,
		TRANSPARENT_PLATE,
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

		MASK_PATCH_CONNECT_TL,
		MASK_PATCH_CONNECT_TR,
		MASK_PATCH_CONNECT_BL,
		MASK_PATCH_CONNECT_BR,
		MASK_PATCH_CONNECT_T,
		MASK_PATCH_CONNECT_B,
		MASK_PATCH_CONNECT_L,
		MASK_PATCH_CONNECT_R,

		MASK_PATCH_CONNECT_INV_TL,
		MASK_PATCH_CONNECT_INV_TR,
		MASK_PATCH_CONNECT_INV_BL,
		MASK_PATCH_CONNECT_INV_BR,
		MASK_PATCH_CONNECT_INV_T,
		MASK_PATCH_CONNECT_INV_B,
		MASK_PATCH_CONNECT_INV_L,
		MASK_PATCH_CONNECT_INV_R,

		MASK_PATCH_SOLO,
		MASK_PATCH_SOLO_T,
		MASK_PATCH_SOLO_B,
		MASK_PATCH_SOLO_L,
		MASK_PATCH_SOLO_R,
		/*MASK_PATCH_SOLO_CONNECT_T,
		MASK_PATCH_SOLO_CONNECT_B,
		MASK_PATCH_SOLO_CONNECT_L,
		MASK_PATCH_SOLO_CONNECT_R,*/
		MASK_PATCH_SOLO_CORNERS,
		MAX_NUM_MASKS
	};
	
	static Vec2 getMinUvFromTileType(TILE_TYPE type)
	{
		switch (type)
		{
			case TRANSPARENT_PLATE: return Vec2(MIN_UV(0, 3)); break;
			case SKY:				return Vec2(MIN_UV(0, 2)); break;
			case WIRE_FRAME:		return Vec2(MIN_UV(0, 1)); break;

			case STONE:				return Vec2(MIN_UV(1, 6)); break;
			case DIRT:				return Vec2(MIN_UV(0, 7)); break;
			case GRASS:				return Vec2(MIN_UV(0, 6)); break;

			case EMPTY:
			default:				return Vec2(MIN_UV(0, 0));
		}
	}

	static Vec2 getMinUvMaskFromTileMask(TILE_MASK type)
	{
		switch (type)
		{
			case MASK_PATCH_TL:				return Vec2(MIN_UV(0, 8)); break;//return Vec2(MIN_UV(2, 4)); break;
			case MASK_PATCH_TR:				return Vec2(MIN_UV(2, 8)); break;//return Vec2(MIN_UV(3, 4)); break;
			case MASK_PATCH_BL:				return Vec2(MIN_UV(0, 10)); break;//return Vec2(MIN_UV(2, 5)); break;
			case MASK_PATCH_BR:				return Vec2(MIN_UV(2, 10)); break;//return Vec2(MIN_UV(3, 5)); break;
			case MASK_PATCH_T:				return Vec2(MIN_UV(1, 8)); break;//return Vec2(MIN_UV_PATCH(2, 4, 1)); break;
			case MASK_PATCH_L:				return Vec2(MIN_UV(0, 9)); break;//return Vec2(MIN_UV_PATCH(2, 4, 0)); break;
			case MASK_PATCH_R:				return Vec2(MIN_UV(2, 9)); break;//return Vec2(MIN_UV_PATCH(3, 4, 0)); break;
			case MASK_PATCH_B:				return Vec2(MIN_UV(1, 10)); break;//return Vec2(MIN_UV_PATCH(2, 5, 1)); break;
			
			case MASK_PATCH_CONNECT_TL:		return Vec2(MIN_UV(3, 8)); break;
			case MASK_PATCH_CONNECT_TR:		return Vec2(MIN_UV(5, 8)); break;
			case MASK_PATCH_CONNECT_BL:		return Vec2(MIN_UV(3, 10)); break;
			case MASK_PATCH_CONNECT_BR:		return Vec2(MIN_UV(5, 10)); break;
			case MASK_PATCH_CONNECT_T:		return Vec2(MIN_UV(4, 8)); break;
			case MASK_PATCH_CONNECT_L:		return Vec2(MIN_UV(3, 9)); break;
			case MASK_PATCH_CONNECT_R:		return Vec2(MIN_UV(5, 10)); break;
			case MASK_PATCH_CONNECT_B:		return Vec2(MIN_UV(4, 9)); break;

			case MASK_PATCH_CONNECT_INV_TL:	return Vec2(MIN_UV(3, 11)); break;
			case MASK_PATCH_CONNECT_INV_TR:	return Vec2(MIN_UV(5, 11)); break;
			case MASK_PATCH_CONNECT_INV_BL:	return Vec2(MIN_UV(3, 13)); break;
			case MASK_PATCH_CONNECT_INV_BR:	return Vec2(MIN_UV(5, 13)); break;
			case MASK_PATCH_CONNECT_INV_T:	return Vec2(MIN_UV(4, 11)); break;
			case MASK_PATCH_CONNECT_INV_L:	return Vec2(MIN_UV(3, 12)); break;
			case MASK_PATCH_CONNECT_INV_R:	return Vec2(MIN_UV(5, 13)); break;
			case MASK_PATCH_CONNECT_INV_B:	return Vec2(MIN_UV(4, 12)); break;

			case MASK_PATCH_SOLO:			return Vec2(MIN_UV(1, 4)); break;
			case MASK_PATCH_SOLO_T:			return Vec2(MIN_UV(2, 6)); break;
			case MASK_PATCH_SOLO_B:			return Vec2(MIN_UV(3, 6)); break;
			case MASK_PATCH_SOLO_L:			return Vec2(MIN_UV(2, 7)); break;
			case MASK_PATCH_SOLO_R:			return Vec2(MIN_UV(3, 7)); break;
			/*case MASK_PATCH_SOLO_CONNECT_T:	return Vec2(MIN_UV(4, 5)); break;
			case MASK_PATCH_SOLO_CONNECT_B:	return Vec2(MIN_UV(5, 5)); break;
			case MASK_PATCH_SOLO_CONNECT_L:	return Vec2(MIN_UV(5, 4)); break;
			case MASK_PATCH_SOLO_CONNECT_R:	return Vec2(MIN_UV(4, 4)); break;*/
			case MASK_PATCH_SOLO_CORNERS:	return Vec2(MIN_UV(1, 5)); break;
			case MASK_PATCH_FULL:			return Vec2(MIN_UV(0, 5)); break;
			case MASK_EMPTY:
			default:						return Vec2(MIN_UV(0, 4));
		}
	}
};

#endif
