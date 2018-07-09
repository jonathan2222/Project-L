#ifndef CHUNK_H
#define CHUNK_H

#include "TerrainConfig.h"
#include "Tile.h"

#include "../../Rendering/Model.h"

class Renderer;
struct Chunk
{
	Chunk();
	~Chunk();

	void createModel(unsigned int layer);
	void draw(const Renderer& renderer, unsigned int layer, bool useWireframe) const;
	void updateLayers();
	void updateLayer(unsigned int layer);

	Tile tiles[TILE_LAYERS][CHUNK_SIZE][CHUNK_SIZE];

	Model* models[TILE_LAYERS];
};

#endif
