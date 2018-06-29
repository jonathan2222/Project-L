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
	Shader * terrainShader;
	Chunk chunks[NUM_CHUNKS_VERTICAL][NUM_CHUNKS_HORIZONTAL];

	Mat3 transform;

	std::vector<Texture*> texturesToDraw;
	std::vector<std::vector<Vec2>> tilesToDraw; // Each batch has its corresponding texture from the textureToDraw vector.
};

#endif
