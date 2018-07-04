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
	void createTileVAO();
	void getTilesToDraw(Display* display);
	void processInput(Display* display); // Temporary, should be in a separate class or in another class.

	Shader * terrainShader;
	Chunk chunks[NUM_CHUNKS_VERTICAL][NUM_CHUNKS_HORIZONTAL];

	Mat3 transform;
	unsigned int maxTilesDrawn;
	std::vector<Vec2> translations;
	std::vector<Vec2> minUvs;
	Camera camera;
};

#endif
