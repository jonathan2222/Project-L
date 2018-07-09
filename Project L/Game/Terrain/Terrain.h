#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../Rendering/Model.h"
#include "../../Rendering/Renderer.h"

#include "../../Maths/Matrices/Mat3.h"
#include "../../GLAbstractions/Texture.h"

#include "Chunk.h"

#include "../Player/Camera.h"

class Display;
class Terrain
{
public:
	Terrain();
	~Terrain();

	void draw(Camera& camera, const Renderer& renderere, Display* display);

	// (v, h, xc, yc)
	Vec4 getChunkIndicesFromPos(float x, float y);
	Tile* getTileFromPos(float x, float y, unsigned int layer);

	Chunk* getChunk(unsigned int v, unsigned int h);
	Tile* getTile(unsigned int v, unsigned int h, unsigned int x, unsigned int y, unsigned int layer);
	bool removeTile(unsigned int v, unsigned int h, unsigned int x, unsigned int y, unsigned int layer);

	bool removeTile(float x, float y, unsigned int layer);
	bool isPosInsideTerrain(float x, float y) const;

	void updateVisibleChunks();

private:
	void drawChunk(Camera& camera, Chunk* chunk, const Renderer& renderer, bool useWireframe);

	void getVisibleChunks(Camera& camera, Display* display);
	bool calculateMask();
	void calculateMask(unsigned int flags, Vec2 minUv, Vec2& minUv2, Vec2& minUvMask, float x, float y, unsigned int layer);
	void calculateDetail();
	void calculateDetail(unsigned int flags, Vec2 minUv, Vec2& minUvLeft, Vec2& minUvRight, Vec2& minUvUp, Vec2& minUvDown, Vec4& maskSide, unsigned int& corners, float x, float y, unsigned int layer);
	
	unsigned int getRandomBits() const;

private:
	Shader * terrainShader;
	Chunk* chunks[NUM_CHUNKS_VERTICAL][NUM_CHUNKS_HORIZONTAL];
	bool recalculateMask;
	unsigned int randomNumber;

	Mat3 transform;
	std::vector<std::pair<unsigned int, unsigned int>> chunksToDraw;
};

#endif
