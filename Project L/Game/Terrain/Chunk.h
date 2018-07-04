#ifndef CHUNK_H
#define CHUNK_H

#include "TerrainConfig.h"
#include "Tile.h"

struct Chunk
{
	Tile tiles[CHUNK_SIZE][CHUNK_SIZE][TILE_LAYERS];
};

#endif
