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
	void drawChunk(Chunk* chunk, const Renderer& renderer, bool useWireframe);

	void getTilesToDraw(Display* display, bool useWireframe);
	void processInput(Display* display); // Temporary, should be in a separate class or in another class.
	void calculateMaskAndType(Vec2& minUv2, Vec2& minUvMask, float x, float y, unsigned int layer);
	void calculateDetail(Vec2 minUv, Vec2& minUvLeft, Vec2& minUvRight, Vec2& minUvUp, Vec2& minUvDown, Vec4& maskSide, unsigned int& corners, float x, float y, unsigned int layer);
	
	// (v, h, xc, yc)
	Vec4 getChunkIndicesFromPos(float x, float y);

	Tile* getTileFromPos(float x, float y, unsigned int layer);
	unsigned int getRandomBits() const;
	void setBitsIfNot(unsigned int& bits, unsigned int bit1, unsigned int bit2) const;

	Shader * terrainShader;
	Chunk* chunks[NUM_CHUNKS_VERTICAL][NUM_CHUNKS_HORIZONTAL];
	bool recalculateMask;
	unsigned int randomNumber;

	Mat3 transform;
	std::vector<std::pair<unsigned int, unsigned int>> chunksToDraw;
	Camera camera;
};

#endif
