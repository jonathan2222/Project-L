#include "Terrain.h"

#include "../../Rendering/ModelManager.h"
#include "../../Utils/ModelCreator.h"
#include "../../ResourceManager.h"

#include "TerrainConfig.h"
#include "../../Maths/Maths.h"
#include "../../Maths/MathsTransform.h"
#include "../../Utils/Utils.h"

#include "../../GUI/Display.h"

Terrain::Terrain()
{
	this->terrainShader = new Shader("Shaders/Terrain/terrainInstanced.fs", "Shaders/Terrain/terrainInstanced.vs");

	ResourceManager::addTexture("Tile Map", "Textures/Terrain/tile_map2.png"); // Map can hold 1024 different 32x32 tiles.

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
					float posX = tile.pos.x + (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;
					float noise = Utils::perlinNoise(posX*0.05f, 0.5f, 2.0f, 4, Utils::COSINE_INTERPOLATION);
					int padding = floor(noise*CHUNK_SIZE*0.7f);
					noise = floor((noise*2.0f - 1.0f)*NUM_CHUNKS_VERTICAL*CHUNK_SIZE*0.25f);
					if (noise < tile.pos.y) tile.type = TileConfig::EMPTY;
					if (noise == tile.pos.y) tile.type = TileConfig::DIRT_GRASS;
					if (noise > tile.pos.y) tile.type = TileConfig::DIRT;
					if (noise- padding == tile.pos.y) tile.type = TileConfig::STONE_DIRT;
					if (noise- padding > tile.pos.y) tile.type = TileConfig::STONE;
					//tile.type = (TileConfig::TILE_TYPE)((h+v+xc+yc)%6);
				}
		}
}

Terrain::~Terrain()
{
	delete this->terrainShader;
}

void Terrain::draw(const Renderer & renderer, Display* display)
{
	static bool useWireframe = false;
	if (Input::isKeyClicked(GLFW_KEY_F))
		useWireframe ^= 1;

	processInput(display);
	camera.setZoom(camera.getZoom(), display->getRatio()); // Adjust aspect ratio.

	Model* m = ModelManager::get("TileModelInstanced");
	this->translations.clear();
	this->minUvs.clear();
	getTilesToDraw(display, useWireframe);
	m->count = this->minUvs.size();
	if (m->count > 0)
	{
		m->vbInstanced.updateData(&this->translations[0][0], sizeof(Vec2)*m->count);
		m->vbInstanced2.updateData(&this->minUvs[0][0], sizeof(Vec2)*m->count);
	}
	else
	{
		m->vbInstanced.updateData(NULL, sizeof(Vec2)*m->count);
		m->vbInstanced2.updateData(NULL, sizeof(Vec2)*m->count);
	}
	
	this->terrainShader->bind();
	this->terrainShader->setUniformMatrix4fv("camera", 1, false, &camera.getMatrix()[0][0]);
	this->terrainShader->setUniform1f("uvScale", (float)(TILE_IMG_SIZE) / TILE_MAP_IMG_SIZE); // TODO: Make this work.
	this->terrainShader->setUniformMatrix3fv("transform", 1, false, &(this->transform[0][0]));
	this->terrainShader->setTexture2D("tex", 0, ResourceManager::getTexture("Tile Map")->getID());
	if (useWireframe)
		renderer.drawInstanced(GL_LINE_LOOP, m->va, m->ib, m->count);
	else
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

void Terrain::getTilesToDraw(Display* display, bool useWireframe)
{
	
	Vec3 camPos = camera.getPosition() / TILE_SIZE;
	float numTilesX = camera.getZoom()*display->getRatio() / TILE_SIZE + 1.0f;
	float numTilesY = camera.getZoom() / TILE_SIZE + 1.0f;
	if ((int)numTilesX % 2 != 0) numTilesX++;
	if ((int)numTilesY % 2 != 0) numTilesY++;
	const float numTiles = numTilesX * numTilesY;
	
	this->translations.reserve(maxTilesDrawn);
	this->minUvs.reserve(maxTilesDrawn);
	const int halfTilesToDrawX = floor(numTilesX / 2.0f);
	const int halfTilesToDrawY = floor(numTilesY / 2.0f);
	for(int x = -halfTilesToDrawX; x <= halfTilesToDrawX; x++)
		for (int y = -halfTilesToDrawY; y <= halfTilesToDrawY; y++)
		{
			const float posX = camPos.x + (float)x;
			const float posY = camPos.y + (float)y;
			const float rightBoundH = (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
			const float leftBoundH = -rightBoundH;
			const float rightBoundV = (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
			const float leftBoundV = -rightBoundV;

			// TODO: REMOVE THIS (Error checking)!! (After adding a player)
			if ((posX > leftBoundH) && (posX < rightBoundH) && (posY > leftBoundV) && (posY < rightBoundV))
			{
				Tile& tile = getTileFromPos(posX, posY);
				if (tile.type != TileConfig::EMPTY)
				{
					if (useWireframe)
						this->minUvs.push_back(TileConfig::getMinUvFromTileType(TileConfig::WIRE_FRAME));
					else
						this->minUvs.push_back(TileConfig::getMinUvFromTileType(tile.type));
					this->translations.push_back(tile.pos*TILE_SIZE);
				}
			}
		}
		
}

// TODO: MOVE THIS!, Temporary, should be in a separate class or in another class.
void Terrain::processInput(Display* display)
{
	if (Input::isScrolling())
	{
		float newZoom = camera.getZoom() + TILE_SIZE * -Input::getScrollYOffest();
		if(newZoom > 0)
			camera.setZoom(newZoom);
		Error::printWarning("Zoom: " + std::to_string(newZoom));
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
		translation.x = -(float)dist.x / display->getWidth() * camera.getZoom() * display->getRatio();
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
		camera.move({ -TILE_SIZE, 0.0f, 0.0f });
	if (Input::isKeyClicked(GLFW_KEY_D))
		camera.move({ TILE_SIZE, 0.0f, 0.0f });
	if (Input::isKeyClicked(GLFW_KEY_W))
		camera.move({ 0.0f, TILE_SIZE, 0.0f });
	if (Input::isKeyClicked(GLFW_KEY_S))
		camera.move({ 0.0f, -TILE_SIZE, 0.0f });

	if (Input::isButtonClicked(GLFW_MOUSE_BUTTON_RIGHT))
	{
		Vec3 camPos = camera.getPosition() / TILE_SIZE;
		float numTilesX = camera.getZoom()*display->getRatio() / TILE_SIZE;
		float numTilesY = camera.getZoom() / TILE_SIZE;
		
		Vec2 mouseOffset(floor(mPos.x/ display->getWidth() * camera.getZoom() * display->getRatio()), floor(mPos.y / display->getHeight() * camera.getZoom()));
		mouseOffset.x -= floor(numTilesX / 2);
		mouseOffset.y -= floor(numTilesY / 2);
		mouseOffset.y = -mouseOffset.y;

		Vec2 tilePos = camPos + mouseOffset;
		std::cout << "Tile pos: " << Utils::toString(tilePos) << std::endl;

		const int rightBoundH = (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
		const int leftBoundH = -rightBoundH;
		const int rightBoundV = (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
		const int leftBoundV = -rightBoundV;

		if (tilePos.x > leftBoundH && tilePos.x < rightBoundH &&
			tilePos.y > leftBoundV && tilePos.y < rightBoundV)
		{
			Tile& tile = getTileFromPos(tilePos.x, tilePos.y);
			//std::cout << "Tile pos: " << Utils::toString(tilePos) << ", type: " << tile.type << std::endl;
		}

	}
}

Tile & Terrain::getTileFromPos(float x, float y)
{
	const float posXT = x + (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;
	const float posYT = y + (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;

	const int h = floor(posXT / CHUNK_SIZE);
	const int v = floor(posYT / CHUNK_SIZE);

	const int xc = (int)posXT % CHUNK_SIZE;
	const int yc = (int)posYT % CHUNK_SIZE;

	return this->chunks[v][h].tiles[yc][xc];
}
