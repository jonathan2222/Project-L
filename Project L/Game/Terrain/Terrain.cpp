#include "Terrain.h"

#include "../../Rendering/ModelManager.h"
#include "../../Utils/ModelCreator.h"
#include "../../ResourceManager.h"

#include "TerrainConfig.h"
#include "../../Maths/Maths.h"
#include "../../Maths/MathsTransform.h"

#include "../../GUI/Display.h"

Terrain::Terrain()
{
	this->terrainShader = new Shader("Shaders/Terrain/terrainInstanced.fs", "Shaders/Terrain/terrainInstanced.vs");

	ResourceManager::addTexture("Tile Map", "Textures/Terrain/tile_map.png"); // Map can hold 1024 different 32x32 tiles.

	camera.setZoom(1.0f);

	// Set the tile size.
	this->transform[0][0] = TILE_SIZE;
	this->transform[1][1] = TILE_SIZE;

	createTileVAO();
	
	// Create Tiles and chunks.
	for (unsigned int v = 0; v < NUM_CHUNKS_VERTICAL; v++)
		for (unsigned int h = 0; h < NUM_CHUNKS_HORIZONTAL; h++)
		{
			Chunk& chunk = this->chunks[v][h];
			for(unsigned int yc = 0; yc < CHUNK_SIZE; yc++)
				for (unsigned int xc = 0; xc < CHUNK_SIZE; xc++)
				{
					Tile& tile = chunk.tiles[yc][xc];
					int cunkOffsetX = (h - (int)(NUM_CHUNKS_HORIZONTAL / 2)) * CHUNK_SIZE - CHUNK_SIZE / 2;
					int cunkOffsetY = (v - (int)(NUM_CHUNKS_VERTICAL / 2)) * CHUNK_SIZE - CHUNK_SIZE /2;
					tile.pos = Vec2((float)xc + (float)cunkOffsetX,
									(float)yc + (float)cunkOffsetY);

					tile.type = (TileConfig::TILE_TYPE)((v + h) % 4);
				}
		}
}

Terrain::~Terrain()
{
	delete this->terrainShader;
}

void Terrain::draw(const Renderer & renderer, Display* display)
{
	Model* m = ModelManager::get("TileModelInstanced");
	this->translations.clear();
	this->minUvs.clear();
	getTilesToDraw();
	m->count = this->minUvs.size();
	m->vbInstanced.updateData(&this->translations[0][0], sizeof(Vec2)*m->count);
	m->vbInstanced2.updateData(&this->minUvs[0][0], sizeof(Vec2)*m->count);

	processInput(display);

	this->terrainShader->bind();
	this->terrainShader->setUniformMatrix4fv("camera", 1, false, &camera.getMatrix()[0][0]);
	this->terrainShader->setUniform1f("uvScale", (float)TILE_IMG_SIZE / TILE_MAP_IMG_SIZE);
	this->terrainShader->setUniformMatrix3fv("transform", 1, false, &(this->transform[0][0]));
	this->terrainShader->setTexture2D("tex", 0, ResourceManager::getTexture("Tile Map")->getID());
	renderer.drawInstanced(m->va, m->ib, m->count);
}

void Terrain::createTileVAO()
{
	Model* model = ModelCreator::createRectangleModel(1.0f, 1.0f);
	model->isInstanced = true;
	model->count = 0;

	// TODO: SET THE RIGHT MAX COUNT OF INSTANCES TO THE AMOUNT OF TITLE IT TAKES TO FILL THE SCREEN.
	this->maxTilesDrawn = 10000;

	// Create buffer for translation
	model->vbInstanced.make(NULL, sizeof(float) * 4 * this->maxTilesDrawn, GL_STREAM_DRAW);
	VertexBufferLayout instancedLayout;
	instancedLayout.push<float>(2); // Translation
	model->va.addBuffer(model->vbInstanced, instancedLayout, true);

	// Create buffer for min uv, use same instanced layout
	model->vbInstanced2.make(NULL, sizeof(float) * 4 * this->maxTilesDrawn, GL_STREAM_DRAW);
	model->va.addBuffer(model->vbInstanced2, instancedLayout, true);

	ModelManager::add("TileModelInstanced", model);
}

void Terrain::getTilesToDraw()
{
	this->translations.reserve(this->maxTilesDrawn);
	this->minUvs.reserve(this->maxTilesDrawn);
	const int TILES_TO_SHOW_RAD = 2;
	for (unsigned int v = 0; v < NUM_CHUNKS_VERTICAL; v++)
		for (unsigned int h = 0; h < NUM_CHUNKS_HORIZONTAL; h++)
		{
			Chunk& chunk = this->chunks[v][h];
			for (unsigned int yc = 0; yc < CHUNK_SIZE; yc++)
				for (unsigned int xc = 0; xc < CHUNK_SIZE; xc++)
				{
					Tile& tile = chunk.tiles[yc][xc];
					//if (tile.pos.x < TILES_TO_SHOW_RAD && tile.pos.x > -TILES_TO_SHOW_RAD &&
					//	tile.pos.y < TILES_TO_SHOW_RAD && tile.pos.y > -TILES_TO_SHOW_RAD)
					//{
						this->minUvs.push_back(TileConfig::getMinUvFromTileType(tile.type));
						this->translations.push_back(tile.pos*TILE_SIZE);
					//}

				}
		}
}

// Temporary, should be in a separate class or in another class.
void Terrain::processInput(Display* display)
{
	if (Input::isScrolling())
	{
		camera.setZoom(camera.getZoom() + /*ZOOM_PER_SCROLL_TICK*/TILE_SIZE * -Input::getScrollYOffest());
	}

	static Vec2 prePos(-1, -1);
	Vec2 dist(0.0, 0.0);
	Vec2 mPos = Input::getMousePosition();
	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (prePos.x == -1 && prePos.y == -1)
			prePos = mPos;

		dist = mPos - prePos;
		Vec3 translation;
		translation.x = -(float)dist.x / display->getWidth() * camera.getZoom();
		translation.y = (float)dist.y / display->getHeight() * camera.getZoom();
		camera.move(translation);

		prePos = mPos;
	}
	else
	{
		prePos.x = -1;
		prePos.y = -1;
	}

	if (Input::isKeyClicked(GLFW_KEY_A))
		std::cout << "A is clicked" << std::endl;
}
