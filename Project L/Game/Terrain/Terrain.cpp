#include "Terrain.h"

#include "../../Rendering/ModelManager.h"
#include "../../Utils/ModelCreator.h"
#include "../../ResourceManager.h"

#include "TerrainConfig.h"

Terrain::Terrain()
{
	this->terrainShader = new Shader("Shaders/Terrain/terrainInstanced.fs", "Shaders/Terrain/terrainInstanced.vs");

	this->transform[0][0] = TILE_SIZE;
	this->transform[1][1] = TILE_SIZE;
	Vec2 translations[CHUNK_SIZE*CHUNK_SIZE];
	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int y = 0; y < CHUNK_SIZE; y++)
		{
			unsigned int index = y * CHUNK_SIZE + x;
			translations[index].x = x * TILE_SIZE - CHUNK_SIZE * TILE_SIZE / 2.0f; // X
			translations[index].y = y * TILE_SIZE - CHUNK_SIZE * TILE_SIZE / 2.0f; // Y
		}
	ModelManager::add("TileModelInstanced", ModelCreator::createRectangleInstancedModel(CHUNK_SIZE*CHUNK_SIZE, translations, GL_STREAM_DRAW, 1.0f, 1.0f));

	/*
	for (unsigned int v = 0; v < NUM_CHUNKS_VERTICAL; v++)
		for (unsigned int h = 0; h < NUM_CHUNKS_HORIZONTAL; h++)
		{

		}*/
}

Terrain::~Terrain()
{
	delete this->terrainShader;
}

void Terrain::draw(const Renderer & renderer)
{
	Model* m = ModelManager::get("TileModelInstanced");
	this->terrainShader->bind();
	this->terrainShader->setUniformMatrix3fv("transform", 1, false, &(this->transform[0][0]));
	this->terrainShader->setTexture2D("tex", 0, ResourceManager::getTexture("DirtTexture")->getID());
	renderer.drawInstanced(m->va, m->ib, CHUNK_SIZE*CHUNK_SIZE);
}
