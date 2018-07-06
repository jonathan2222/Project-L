#ifndef TILE_H
#define TILE_H

#include "../../Maths/Vectors/Vec2.h"
#include "../../Maths/Vectors/Vec4.h"
#include "TileConfig.h"

struct Tile
{
	Vec2 pos;
	TileConfig::TILE_TYPE type = TileConfig::EMPTY;
	TileConfig::TILE_TYPE type2 = TileConfig::EMPTY;
	TileConfig::TILE_TYPE typeLeft = TileConfig::EMPTY;
	TileConfig::TILE_TYPE typeRight = TileConfig::EMPTY;
	TileConfig::TILE_TYPE typeUp = TileConfig::EMPTY;
	TileConfig::TILE_TYPE typeDown = TileConfig::EMPTY;
	TileConfig::TILE_MASK mask = TileConfig::MASK_PATCH_FULL;
	Vec4 maskSide = Vec4(0.0);
	unsigned int randomBits = 0;
};

#endif
