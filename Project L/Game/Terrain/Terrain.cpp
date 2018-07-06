#include "Terrain.h"

#include "../../Rendering/ModelManager.h"
#include "../../Utils/ModelCreator.h"
#include "../../ResourceManager.h"

#include "TerrainConfig.h"
#include "../../Maths/Maths.h"
#include "../../Maths/MathsTransform.h"
#include "../../Utils/Utils.h"

#include "../../GUI/Display.h"
#include <cassert>

Terrain::Terrain()
{
	this->terrainShader = new Shader("Shaders/Terrain/terrainInstanced.fs", "Shaders/Terrain/terrainInstanced.vs");

	ResourceManager::addTexture("Tile Map", "Textures/Terrain/tile_map_8x8.png"); // Map can hold 1024 different 32x32 tiles or 4096 16x16 tiles.

	camera.setZoom(18.0f);

	// Set the tile size.
	this->transform[0][0] = TILE_SIZE;
	this->transform[1][1] = TILE_SIZE;

	createTileVAO();
	
	// Create Tiles and chunks.
	for (unsigned int v = 0; v < NUM_CHUNKS_VERTICAL; v++)
		for (unsigned int h = 0; h < NUM_CHUNKS_HORIZONTAL; h++)
		{
			this->chunks[v][h] = new Chunk();
			Chunk& chunk = *this->chunks[v][h];
			for(unsigned int yc = 0; yc < CHUNK_SIZE; yc++)
				for (unsigned int xc = 0; xc < CHUNK_SIZE; xc++)
				{
					Tile& tile = chunk.tiles[yc][xc][MIDDLE_TILE]; // Set the middle tile.
					int cunkOffsetX = (h - (int)(NUM_CHUNKS_HORIZONTAL / 2)) * CHUNK_SIZE - CHUNK_SIZE / 2;
					int cunkOffsetY = (v - (int)(NUM_CHUNKS_VERTICAL / 2)) * CHUNK_SIZE - CHUNK_SIZE /2;
					tile.pos = Vec2((float)xc + (float)cunkOffsetX,
									(float)yc + (float)cunkOffsetY);
					float posX = tile.pos.x + (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;
					float noise = Utils::perlinNoise(posX*0.05f, 0.5f, 2.0f, 4, Utils::COSINE_INTERPOLATION);
					int padding = floor(noise*CHUNK_SIZE*0.7f);
					noise = floor((noise*2.0f - 1.0f)*NUM_CHUNKS_VERTICAL*CHUNK_SIZE*0.25f);
					if (noise < tile.pos.y) tile.type = TileConfig::EMPTY;
					if (noise == tile.pos.y) tile.type = TileConfig::GRASS;
					else if (noise > tile.pos.y) tile.type = TileConfig::DIRT;
					if (noise- padding > tile.pos.y) tile.type = TileConfig::STONE;
					//tile.type = (TileConfig::TILE_TYPE)((h+v+xc+yc)%(TileConfig::MAX_NUM_TYPES));

					tile.mask = TileConfig::MASK_EMPTY; // Init mask to empty.

					Tile& background = chunk.tiles[yc][xc][BACK_TILE];
					background.pos = tile.pos;
					background.type = TileConfig::SKY;
					background.mask = TileConfig::MASK_EMPTY;

					/*Tile& foreground = chunk.tiles[yc][xc][FRONT_TILE];
					foreground.pos = background.pos;
					if (noise < foreground.pos.y) foreground.type = TileConfig::EMPTY;
					else foreground.type = TileConfig::TRANSPARENT_PLATE;*/
				}
		}
}

Terrain::~Terrain()
{
	for (unsigned int v = 0; v < NUM_CHUNKS_VERTICAL; v++)
		for (unsigned int h = 0; h < NUM_CHUNKS_HORIZONTAL; h++)
			delete this->chunks[v][h];
	delete this->terrainShader;
}

void Terrain::draw(const Renderer & renderer, Display* display)
{
	static bool useWireframe = false;
	if (Input::isKeyClicked(GLFW_KEY_F))
		useWireframe ^= 1;

	processInput(display);
	camera.setZoom(camera.getZoom(), display->getRatio()); // Adjust aspect ratio.

	getTilesToDraw(display, useWireframe);

	unsigned int i = MIDDLE_TILE;
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
	{
		drawLayer(renderer, i, useWireframe);
		this->translations[i].clear();
		this->minUvs[i].clear();
	}
}

void Terrain::drawLayer(const Renderer & renderer, unsigned int layer, bool useWireframe)
{
	Model* m = ModelManager::get("TileModelInstanced" + std::to_string(layer));
	m->count = this->minUvs[layer].size();
	if (m->count > 0)
	{
		m->vbInstanced.updateData(&this->translations[layer][0][0], sizeof(Vec2)*m->count);
		m->vbInstanced2.updateData(&this->minUvs[layer][0].minUv, sizeof(TileDrawData)*m->count);
	}
	else
	{
		m->vbInstanced.updateData(NULL, sizeof(Vec2)*m->count);
		m->vbInstanced2.updateData(NULL, sizeof(TileDrawData)*m->count);
	}

	this->terrainShader->bind();
	this->terrainShader->setUniformMatrix4fv("camera", 1, false, &camera.getMatrix()[0][0]);
	this->terrainShader->setUniform1i("tileSize", TILE_IMG_SIZE);
	this->terrainShader->setUniformMatrix3fv("transform", 1, false, &(this->transform[0][0]));
	this->terrainShader->setTexture2D("tex", 0, ResourceManager::getTexture("Tile Map")->getID());
	if (useWireframe)
		renderer.drawInstanced(GL_LINE_LOOP, m->va, m->ib, m->count);
	else
		renderer.drawInstanced(m->va, m->ib, m->count);
}

void Terrain::createTileVAO()
{
	// TODO: SET THE RIGHT MAX COUNT OF INSTANCES TO THE AMOUNT OF TILES IT TAKES TO FILL THE SCREEN.
	this->maxTilesDrawn = 10000;
	for(unsigned int i = 0; i < TILE_LAYERS; i++)
		createModel("TileModelInstanced" + std::to_string(i), this->maxTilesDrawn);
}

void Terrain::createModel(const std::string & name, unsigned int maxSize)
{
	Model* model = ModelCreator::createRectangleModel(1.0f, 1.0f, TILE_IMG_SIZE);
	model->isInstanced = true;
	model->count = 0;

	// Create buffer for translation
	model->vbInstanced.make(NULL, sizeof(float) * 2 * maxSize, GL_STREAM_DRAW);
	VertexBufferLayout instancedLayout;
	instancedLayout.push<float>(2); // Translation
	model->va.addBuffer(model->vbInstanced, instancedLayout, true);

	// Create buffer for min uv, use same instanced layout
	model->vbInstanced2.make(NULL, sizeof(float) * 12 * maxSize, GL_STREAM_DRAW);
	VertexBufferLayout instancedLayoutMinUv;
	instancedLayoutMinUv.push<float>(2); // Min uv
	instancedLayoutMinUv.push<float>(2); // Min uv 2 (Second type)
	instancedLayoutMinUv.push<float>(2); // Min uv mask
	instancedLayoutMinUv.push<float>(2); // Min uv mask2
	instancedLayoutMinUv.push<float>(4); // Mask2 dir (Which corner) (tl, tr, bl, br)
	model->va.addBuffer(model->vbInstanced2, instancedLayoutMinUv, true);

	ModelManager::add(name, model);
}

void Terrain::getTilesToDraw(Display* display, bool useWireframe)
{
	
	Vec3 camPos = camera.getPosition() / TILE_SIZE;
	float numTilesX = camera.getZoom()*display->getRatio() / TILE_SIZE + 1.0f;
	float numTilesY = camera.getZoom() / TILE_SIZE + 1.0f;
	if ((int)numTilesX % 2 != 0) numTilesX++;
	if ((int)numTilesY % 2 != 0) numTilesY++;
	const float numTiles = numTilesX * numTilesY * TILE_LAYERS;		// Not used
	
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
	{
		this->translations[i].reserve(numTiles);
		this->minUvs[i].reserve(numTiles);
	}
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
				for (unsigned int i = 0; i < TILE_LAYERS; i++)
				{
					Tile* tile = getTileFromPos(posX, posY, i);
					if (tile != nullptr && tile->type != TileConfig::EMPTY)
					{
						TileDrawData tileData;
						if (useWireframe)
						{
							Vec2 wirePos = TileConfig::getMinUvFromTileType(TileConfig::WIRE_FRAME);
							tileData.minUv = wirePos;
							tileData.minUv2 = wirePos;
							tileData.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);
							tileData.minUvMask2 = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);
							tileData.mask2Dir = Vec4(0,0,0,0);
						}
						else
						{
							tileData.minUv = TileConfig::getMinUvFromTileType(tile->type);
							if (tile->mask == TileConfig::MASK_EMPTY) // If the mask is not set, calculate it.
							{
								TileConfig::TILE_MASK mask;
								TileConfig::TILE_MASK mask2 = TileConfig::MASK_PATCH_FULL;
								Vec4 mask2Dir(0.0f, 0.0f, 0.0f, 0.0f);
								TileConfig::TILE_TYPE type;
								calculateMaskAndType2(mask, type, mask2, mask2Dir, posX, posY, i);
								tileData.minUv2 = TileConfig::getMinUvFromTileType(type);
								tileData.minUvMask = TileConfig::getMinUvMaskFromTileMask(mask);
								tileData.minUvMask2 = TileConfig::getMinUvMaskFromTileMask(mask2);
								tileData.mask2Dir = mask2Dir;
							}
							
						}
						this->minUvs[i].push_back(tileData);
						this->translations[i].push_back(tile->pos*TILE_SIZE);
					}
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
		
		Vec2 mouseOffset(mPos.x/ display->getWidth() * camera.getZoom() * display->getRatio(), mPos.y / display->getHeight() * camera.getZoom());
		mouseOffset.x -= numTilesX / 2.f - TILE_SIZE / 2.0f;
		mouseOffset.y -= numTilesY / 2.f + TILE_SIZE / 2.0f;
		mouseOffset.y = -mouseOffset.y;
		mouseOffset.y = floor(mouseOffset.y);
		mouseOffset.x = floor(mouseOffset.x);

		Vec2 tilePos = camPos + mouseOffset;

		const int rightBoundH = (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
		const int leftBoundH = -rightBoundH;
		const int rightBoundV = (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
		const int leftBoundV = -rightBoundV;

		if (tilePos.x > leftBoundH && tilePos.x < rightBoundH &&
			tilePos.y > leftBoundV && tilePos.y < rightBoundV)
		{
			Tile* tile = getTileFromPos(tilePos.x, tilePos.y, MIDDLE_TILE);
			if(tile != nullptr)
				std::cout << "Tile pos: " << Utils::toString(tilePos) << ", type: " << tile->type << std::endl;
		}

	}
}

void Terrain::calculateMaskAndType2(TileConfig::TILE_MASK& mask, TileConfig::TILE_TYPE& type2, TileConfig::TILE_MASK& mask2, Vec4& mask2Dir, float x, float y, unsigned int layer)
{
	Tile* down = getTileFromPos(x, y - 1, layer);
	Tile* up = getTileFromPos(x, y + 1, layer);
	Tile* left = getTileFromPos(x - 1, y, layer);
	Tile* right = getTileFromPos(x + 1, y, layer);
	if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		Tile* tl = getTileFromPos(x - 1, y + 1, layer);
		Tile* tr = getTileFromPos(x + 1, y + 1, layer);
		Tile* bl = getTileFromPos(x - 1, y - 1, layer);
		Tile* br = getTileFromPos(x + 1, y - 1, layer);
		mask = TileConfig::MASK_PATCH_FULL;
		mask2 = TileConfig::MASK_PATCH_SOLO_CORNERS;
		
		static TileConfig::TILE_TYPE typeArr[8] = { TileConfig::EMPTY };
		typeArr[0] = down->type;
		typeArr[1] = left->type;
		typeArr[2] = right->type;

		if (TILE_GONE(tl))
		{
			mask2Dir.x = 1;
			typeArr[3] = TileConfig::EMPTY;
		}
		else typeArr[3] = tl->type;
		if (TILE_GONE(tr))
		{
			mask2Dir.y = 1;
			typeArr[4] = TileConfig::EMPTY;
		}
		else typeArr[4] = tr->type;
		if (TILE_GONE(bl))
		{
			mask2Dir.z = 1;
			typeArr[5] = TileConfig::EMPTY;
		}
		else typeArr[5] = bl->type;
		if (TILE_GONE(br))
		{
			mask2Dir.w = 1;
			typeArr[6] = TileConfig::EMPTY;
		}
		else typeArr[6] = br->type;

		typeArr[7] = up->type; // if the same number of occured types, this will be used.
		
		// Get least frequent type (excluding EMPTY) around this tile and set that as the second type.
		int counter = 9;
		TileConfig::TILE_TYPE type = TileConfig::EMPTY;
		TileConfig::TILE_TYPE currType = type;
		unsigned int typesOccured[TileConfig::TILE_TYPE::MAX_NUM_TYPES] = { 0 };
		for (unsigned int i = 0; i < 8; i++)
			typesOccured[(TileConfig::TILE_TYPE)typeArr[i]]++;
		for (unsigned int i = 0; i < 8; i++)
		{
			currType = (TileConfig::TILE_TYPE)typeArr[i];
			if (typesOccured[currType] <= counter && currType != 0)
			{
				counter = typesOccured[currType];
				type = currType;
			}
		}
		type2 = type;
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		mask = TileConfig::TILE_MASK::MASK_PATCH_T;
		type2 = down->type;
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		mask = TileConfig::TILE_MASK::MASK_PATCH_TL;
		type2 = down->type;
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		mask = TileConfig::TILE_MASK::MASK_PATCH_TR;
		type2 = down->type;
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_GONE(right))
	{
		mask = TileConfig::TILE_MASK::MASK_PATCH_SOLO_T;
		type2 = down->type;
		return;
	}

	mask = TileConfig::TILE_MASK::MASK_EMPTY;
	type2 = TileConfig::TILE_TYPE::EMPTY;
	return;
}

Tile* Terrain::getTileFromPos(float x, float y, unsigned int layer)
{
	const float posXT = x + (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;
	const float posYT = y + (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;

	const int h = floor(posXT / CHUNK_SIZE);
	const int v = floor(posYT / CHUNK_SIZE);

	const int xc = (int)posXT % CHUNK_SIZE;
	const int yc = (int)posYT % CHUNK_SIZE;

	if (!(v < NUM_CHUNKS_VERTICAL && h < NUM_CHUNKS_HORIZONTAL && v >= 0 && h >= 0))
		return nullptr;
	if (!(xc < CHUNK_SIZE && yc < CHUNK_SIZE && xc >= 0 && yc >= 0))
		return nullptr;

	return &this->chunks[v][h]->tiles[yc][xc][layer];
}
