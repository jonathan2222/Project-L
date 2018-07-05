#ifndef TILE_CONFIG_H
#define TILE_CONFIG_H

#define TILE_IMG_SIZE 16
#define TILE_MAP_IMG_SIZE 1024
#define MIN_UV(indexX, indexY) Vec2((float)(indexX*TILE_IMG_SIZE), (float)(indexY*TILE_IMG_SIZE))

struct TileConfig
{
	enum TILE_TYPE {
		EMPTY = 0,
		DIRT_GRASS = 1,
		DIRT_GRASS_SLOPE,
		DIRT,
		STONE_DIRT,
		STONE,
		WIRE_FRAME,
		SKY,
		TRANSPARENT_PLATE
	};
	
	static Vec2 getMinUvFromTileType(TILE_TYPE type)
	{
		switch (type)
		{
			case TRANSPARENT_PLATE: return Vec2(MIN_UV(0, 3)); break;
			case SKY:				return Vec2(MIN_UV(0, 2)); break;
			case WIRE_FRAME:		return Vec2(MIN_UV(0, 1)); break;
			case STONE:				return Vec2(MIN_UV(1, 3)); break;
			case STONE_DIRT:		return Vec2(MIN_UV(1, 2)); break;
			case DIRT:				return Vec2(MIN_UV(1, 1)); break;
			case DIRT_GRASS_SLOPE:	return Vec2(MIN_UV(2, 0)); break;
			case DIRT_GRASS:		return Vec2(MIN_UV(1, 0)); break;
			case EMPTY:
			default:				return Vec2(MIN_UV(0, 0));
		}
	}
};

#endif
