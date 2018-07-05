#ifndef TILE_H
#define TILE_H

#include "../../Maths/Vectors/Vec2.h"
#include "TileConfig.h"

struct Tile
{
	Vec2 pos;
	TileConfig::TILE_TYPE type = TileConfig::EMPTY;
	TileConfig::TILE_MASK mask = TileConfig::MASK_PATCH_FULL;
};

#endif
