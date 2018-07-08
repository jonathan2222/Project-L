#ifndef TILE_H
#define TILE_H

#include "../../Maths/Vectors/Vec2.h"
#include "../../Maths/Vectors/Vec4.h"
#include "TileConfig.h"

struct Tile
{
	Vec2 pos;
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
