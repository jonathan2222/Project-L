#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../Rendering/Model.h"
#include "../../Rendering/Renderer.h"

#include "../../Maths/Matrices/Mat3.h"
#include "../../GLAbstractions/Texture.h"

#include "Chunk.h"

#include "../Player/Camera.h"

// TODO: remove from this class
#include "../../Input/Input.h"

class Display;
class Terrain
{
public:
	Terrain();
	~Terrain();

	void draw(const Renderer& renderere, Display* display);

private:
	void drawLayer(const Renderer& renderer, unsigned int layer, bool useWireframe);

	void createTileVAO();
	void createModel(const std::string& name, unsigned int maxSize);
	void getTilesToDraw(Display* display, bool useWireframe);
	void processInput(Display* display); // Temporary, should be in a separate class or in another class.
	void calculateMaskAndType2(TileConfig::TILE_MASK& mask, TileConfig::TILE_TYPE& type2, TileConfig::TILE_MASK& mask2, Vec4& mask2Dir, float x, float y, unsigned int layer);
	Tile* getTileFromPos(float x, float y, unsigned int layer);

	Shader * terrainShader;
	Chunk* chunks[NUM_CHUNKS_VERTICAL][NUM_CHUNKS_HORIZONTAL];

	Mat3 transform;
	unsigned int maxTilesDrawn;
	struct TileDrawData
	{
		Vec2 minUv;
		Vec2 minUv2;
		Vec2 minUvMask;
		Vec2 minUvMask2;
		Vec4 mask2Dir;
	};
	std::vector<Vec2> translations[TILE_LAYERS];
	std::vector<TileDrawData> minUvs[TILE_LAYERS];
	Camera camera;
};

#endif
