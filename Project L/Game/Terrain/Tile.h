#ifndef TILE_H
#define TILE_H

#include "../../Maths/Vectors/Vec2.h"
#include "../../Maths/Vectors/Vec4.h"
#include "TileConfig.h"

struct Tile
{
	Vec2 pos;
	/*TileConfig::TILE_TYPE type = TileConfig::TILE_EMPTY;
	TileConfig::TILE_TYPE type2 = TileConfig::TILE_EMPTY;
	TileConfig::TILE_TYPE typeLeft = TileConfig::TILE_EMPTY;
	TileConfig::TILE_TYPE typeRight = TileConfig::TILE_EMPTY;
	TileConfig::TILE_TYPE typeUp = TileConfig::TILE_EMPTY;
	TileConfig::TILE_TYPE typeDown = TileConfig::TILE_EMPTY;
	TileConfig::TILE_MASK mask = TileConfig::MASK_PATCH_FULL;*/
	Vec2 minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	Vec2 minUv2 = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	Vec2 minUvLeft = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	Vec2 minUvRight = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	Vec2 minUvUp = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	Vec2 minUvDown = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	Vec2 minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);
	Vec4 maskSide = Vec4(0.0);
	unsigned int randomBits = 0;
	unsigned int corners = 0;
};

#endif
