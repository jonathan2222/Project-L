#include "Terrain.h"

#include "../../Rendering/ModelManager.h"
#include "../../Utils/ModelCreator.h"
#include "../../ResourceManager.h"

#include "TerrainConfig.h"

Terrain::Terrain()
{
	this->terrainShader = new Shader("Shaders/Terrain/terrainInstanced.fs", "Shaders/Terrain/terrainInstanced.vs");

	ResourceManager::addTexture("Tile Map", "Textures/Terrain/tile_map.png"); // Map can hold 1024 different 32x32 tiles.

	// Set the tile size.
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

	Model* model = ModelCreator::createRectangleModel(1.0f, 1.0f);
	model->isInstanced = true;
	model->count = 0;

	// TODO: SET THE RIGHT MAX COUNT OF INSTANCES TO THE AMOUNT OF TITLE IT TAKES TO FILL THE SCREEN.
	this->maxTilesDrawn = CHUNK_SIZE* CHUNK_SIZE;
	
	model->vbInstanced.make(NULL, sizeof(float) * 4 * this->maxTilesDrawn, GL_STREAM_DRAW);
	VertexBufferLayout instancedLayout;
	instancedLayout.push<float>(2); // Translation
	model->va.addBuffer(model->vbInstanced, instancedLayout, true);
	
	// Same but for min uv.
	model->vbInstanced2.make(NULL, sizeof(float) * 4 * this->maxTilesDrawn, GL_STREAM_DRAW);
	model->va.addBuffer(model->vbInstanced2, instancedLayout, true);

	ModelManager::add("TileModelInstanced", model);

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
	this->translations.clear();
	this->minUvs.clear();
	getTilesToDraw();
	m->count = this->minUvs.size();
	m->vbInstanced.updateData(&this->translations[0][0], sizeof(Vec2)*m->count);
	m->vbInstanced2.updateData(&this->minUvs[0][0], sizeof(Vec2)*m->count);

	this->terrainShader->bind();
	this->terrainShader->setUniform1f("uvScale", (float)TILE_IMG_SIZE / TILE_MAP_IMG_SIZE);
	this->terrainShader->setUniformMatrix3fv("transform", 1, false, &(this->transform[0][0]));
	this->terrainShader->setTexture2D("tex", 0, ResourceManager::getTexture("Tile Map")->getID());
	renderer.drawInstanced(m->va, m->ib, m->count);
}

void Terrain::getTilesToDraw()
{
	this->translations.reserve(CHUNK_SIZE*CHUNK_SIZE);
	this->minUvs.reserve(CHUNK_SIZE*CHUNK_SIZE);
	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int y = 0; y < CHUNK_SIZE; y++)
		{
			this->minUvs.push_back(TileConfig::getMinUvFromTileType((TileConfig::TILE_TYPE)((x+y)%4)));
			this->translations.push_back(Vec2(x * TILE_SIZE - CHUNK_SIZE * TILE_SIZE / 2.0f, 
											  y * (TILE_SIZE-0.001f) - CHUNK_SIZE * TILE_SIZE / 2.0f));
		}
}
