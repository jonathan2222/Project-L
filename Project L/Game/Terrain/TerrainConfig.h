#ifndef TERRAIN_CONFIG_H
#define TERRAIN_CONFIG_H

#define NUM_CHUNKS_HORIZONTAL 50
#define NUM_CHUNKS_VERTICAL 50
#define CHUNK_SIZE 16
#define TILE_LAYERS 3
#define TILE_SIZE 1.0f

#define BACK_TILE 0
#define FRONT_TILE (TILE_LAYERS-1)
#define MIDDLE_TILE (int)(TILE_LAYERS/2.f)

#define TILE_EXIST(tile) (tile != nullptr && tile->minUv != TileConfig::TILE_UV_EMPTY)
#define TILE_GONE(tile) (tile == nullptr || tile->minUv == TileConfig::TILE_UV_EMPTY)

#endif
