#ifndef TILE_CONFIG_H
#define TILE_CONFIG_H

#define TILE_IMG_SIZE 32
#define TILE_MAP_IMG_SIZE 1024
#define MIN_UV(indexX, indexY) Vec2(((float)indexX*TILE_IMG_SIZE)/(float)TILE_MAP_IMG_SIZE, \
									((float)indexY*TILE_IMG_SIZE)/(float)TILE_MAP_IMG_SIZE)

struct TileConfig
{
	enum TILE_TYPE {
		EMPTY = 0,
		DIRT_GRASS = 1,
		GRASS_LOG,
		STONE
	};
	
	static Vec2 getMinUvFromTileType(TILE_TYPE type)
	{
		switch (type)
		{
			case STONE:			return Vec2(MIN_UV(3, 0)); break;
			case GRASS_LOG:		return Vec2(MIN_UV(2, 0)); break;
			case DIRT_GRASS:	return Vec2(MIN_UV(1, 0)); break;
			case EMPTY:
			default:			return Vec2(MIN_UV(0, 0));
		}
	}
};

#endif
