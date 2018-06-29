#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../Rendering/Model.h"
#include "../../Rendering/Renderer.h"

#include "../../Maths/Matrices/Mat3.h"
#include "../../GLAbstractions/Texture.h"

#include "Chunk.h"

class Terrain
{
public:
	Terrain();
	~Terrain();

	void draw(const Renderer& renderere);

private:
	void getTilesToDraw();

	Shader * terrainShader;
	Chunk chunks[NUM_CHUNKS_VERTICAL][NUM_CHUNKS_HORIZONTAL];

	Mat3 transform;
	unsigned int maxTilesDrawn;
	std::vector<Vec2> translations;
	std::vector<Vec2> minUvs;
};

#endif
