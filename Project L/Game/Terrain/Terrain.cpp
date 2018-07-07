#include "Terrain.h"

#include "../../Rendering/ModelManager.h"
#include "../../Utils/ModelCreator.h"
#include "../../ResourceManager.h"

#include "TerrainConfig.h"
#include "../../Maths/Maths.h"
#include "../../Maths/MathsTransform.h"
#include "../../Utils/Utils.h"
#include "../../Utils/BitManipulation.h"

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

	this->recalculateMask = false;
	this->randomNumber = rand();
	
	// Create Tiles and chunks.
	for (unsigned int v = 0; v < NUM_CHUNKS_VERTICAL; v++)
		for (unsigned int h = 0; h < NUM_CHUNKS_HORIZONTAL; h++)
		{
			this->chunks[v][h] = new Chunk();
			Chunk& chunk = *this->chunks[v][h];
			this->chunksToDraw.push_back({ v, h });
			for(unsigned int yc = 0; yc < CHUNK_SIZE; yc++)
				for (unsigned int xc = 0; xc < CHUNK_SIZE; xc++)
				{
					Tile& tile = chunk.tiles[MIDDLE_TILE][yc][xc]; // Set the middle tile.
					int cunkOffsetX = (h - (int)(NUM_CHUNKS_HORIZONTAL / 2)) * CHUNK_SIZE - CHUNK_SIZE / 2;
					int cunkOffsetY = (v - (int)(NUM_CHUNKS_VERTICAL / 2)) * CHUNK_SIZE - CHUNK_SIZE /2;
					tile.pos = Vec2((float)xc + (float)cunkOffsetX,
									(float)yc + (float)cunkOffsetY);
					float posX = tile.pos.x + (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;
					float noise = Utils::perlinNoise(posX*0.05f, 0.5f, 2.0f, 4, Utils::COSINE_INTERPOLATION);
					int padding = floor(noise*CHUNK_SIZE*0.7f);
					noise = floor((noise*2.0f - 1.0f)*NUM_CHUNKS_VERTICAL*CHUNK_SIZE*0.25f);
					if (noise < tile.pos.y) tile.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
					if (noise == tile.pos.y) tile.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_GRASS);
					else if (noise > tile.pos.y) tile.minUv =TileConfig::getMinUvFromTileType(TileConfig::TILE_DIRT);
					if (noise- padding > tile.pos.y) tile.minUv = TileConfig::getMinUvFromTileType(rand()%5 == 0 ? TileConfig::TILE_STONE_2 : TileConfig::TILE_STONE);
					//tile.minUv = TileConfig::getMinUvFromTileType((TileConfig::TILE_TYPE)((h+v+xc+yc)%(TileConfig::MAX_NUM_TYPES)));

					tile.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);// TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY); // Init mask to empty.
					
					Tile& background = chunk.tiles[BACK_TILE][yc][xc];
					background.pos = tile.pos;
					background.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_SKY);
					background.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);
					
					/*
					Tile& foreground = chunk.tiles[FRONT_TILE][yc][xc];
					foreground.pos = tile.pos;
					foreground.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
					foreground.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);
					*/
				}
		}
	for (const std::pair<unsigned int, unsigned int>& chunkIndex : this->chunksToDraw)
		this->chunks[chunkIndex.first][chunkIndex.second]->updateLayers();
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

	for (const std::pair<unsigned int, unsigned int>& chunkIndex : this->chunksToDraw)
		drawChunk(this->chunks[chunkIndex.first][chunkIndex.second], renderer, useWireframe);
}

void Terrain::drawChunk(Chunk* chunk, const Renderer& renderer, bool useWireframe)
{
	this->terrainShader->bind();
	this->terrainShader->setUniformMatrix4fv("camera", 1, false, &camera.getMatrix()[0][0]);
	this->terrainShader->setUniform1i("tileSize", TILE_IMG_SIZE);
	this->terrainShader->setUniformMatrix3fv("transform", 1, false, &(this->transform[0][0]));
	this->terrainShader->setTexture2D("tex", 0, ResourceManager::getTexture("Tile Map")->getID());
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
		chunk->draw(renderer, i, useWireframe);
}

void Terrain::getTilesToDraw(Display* display, bool useWireframe)
{
	bool calcDetail = false;
	/*Vec3 camPos = camera.getPosition() / TILE_SIZE;
	float numTilesX = camera.getZoom()*display->getRatio() / TILE_SIZE + 1.0f;
	float numTilesY = camera.getZoom() / TILE_SIZE + 1.0f;
	if ((int)round(numTilesX) % 2 != 0) numTilesX++;
	if ((int)round(numTilesY) % 2 != 0) numTilesY++;
	numTilesX = round(numTilesX);
	numTilesY = round(numTilesY);
	const float numTiles = numTilesX * numTilesY * TILE_LAYERS;

	const float posX = camPos.x;
	const float posY = camPos.y;
	*/
	static const Vec2 TILE_UV_EMPTY = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	static const Vec2 MASK_UV_EMPTY = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);
	
	for (const std::pair<unsigned int, unsigned int>& chunkIndex : this->chunksToDraw)
	{
		Chunk* chunk = this->chunks[chunkIndex.first][chunkIndex.second];
		for (unsigned int i = 0; i < TILE_LAYERS; i++)
		{
			for (unsigned int yc = 0; yc < CHUNK_SIZE; yc++)
			{
				for (unsigned int xc = 0; xc < CHUNK_SIZE; xc++)
				{
					Tile& tile = chunk->tiles[i][xc][yc];
					if (tile.minUv != TILE_UV_EMPTY)
					{
						if ((tile.minUvMask == MASK_UV_EMPTY) || this->recalculateMask)
						{
							calcDetail = true;
							Vec2 minUvMask;
							Vec2 minUv2 = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);

							calculateMaskAndType(minUv2, minUvMask, tile.pos.x, tile.pos.y, i);

							tile.minUv2 = minUv2;
							tile.minUvLeft = TILE_UV_EMPTY;
							tile.minUvRight = TILE_UV_EMPTY;
							tile.minUvUp = TILE_UV_EMPTY;
							tile.minUvDown = TILE_UV_EMPTY;
							tile.minUvMask = minUvMask;

							tile.maskSide = Vec4(0, 0, 0, 0);
							tile.randomBits = getRandomBits();
							tile.corners = 0;
						}
					}
				}
			}
		}
	}
	if (calcDetail)
	{
		for (const std::pair<unsigned int, unsigned int>& chunkIndex : this->chunksToDraw)
		{
			Chunk* chunk = this->chunks[chunkIndex.first][chunkIndex.second];
			for (unsigned int i = 0; i < TILE_LAYERS; i++)
			{
				for (unsigned int xc = 0; xc < CHUNK_SIZE; xc++)
				{
					for (unsigned int yc = 0; yc < CHUNK_SIZE; yc++)
					{
						Tile& tile = chunk->tiles[i][xc][yc];
						if (tile.minUv != TILE_UV_EMPTY)
						{
							Vec2 minUvLeft = TILE_UV_EMPTY;
							Vec2 minUvRight = TILE_UV_EMPTY;
							Vec2 minUvUp = TILE_UV_EMPTY;
							Vec2 minUvDown = TILE_UV_EMPTY;
							Vec4 maskSide;
							unsigned int corners = 0;
							calculateDetail(tile.minUv, minUvLeft, minUvRight, minUvUp, minUvDown, maskSide, corners, tile.pos.x, tile.pos.y, i);
							tile.minUvLeft = minUvLeft;
							tile.minUvRight = minUvRight;
							tile.minUvUp = minUvUp;
							tile.minUvDown = minUvDown;
							tile.maskSide = maskSide;
							tile.corners = corners;
						}
					}
				}
			}
		}
		for (const std::pair<unsigned int, unsigned int>& chunkIndex : this->chunksToDraw)
			this->chunks[chunkIndex.first][chunkIndex.second]->updateLayers();
	}

	this->recalculateMask = false;
	/*
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
					if (tile != nullptr && tile->type != TileConfig::TILE_EMPTY)
					{
						TileDrawData tileData;
						if (useWireframe)
						{
							Vec2 wirePos = TileConfig::getMinUvFromTileType(TileConfig::TILE_WIRE_FRAME);
							tileData.minUv = wirePos;
							tileData.minUv2 = wirePos;
							tileData.minUvLeft = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
							tileData.minUvRight = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
							tileData.minUvUp = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
							tileData.minUvDown = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
							tileData.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);
							tileData.maskSide = Vec4(0,0,0,0);
							tileData.randomBits = 0;
						}
						else
						{
							tileData.minUv = TileConfig::getMinUvFromTileType(tile->type);
							if (tile->mask == TileConfig::MASK_EMPTY || this->recalculateMask) // If the mask is not set, calculate it.
							{
								calcDetail = true;
								TileConfig::TILE_MASK mask;
								TileConfig::TILE_TYPE type2 = TileConfig::TILE_EMPTY;
								
								calculateMaskAndType(tile->type, type2, mask, posX, posY, i);

								tileData.minUv2 = TileConfig::getMinUvFromTileType(type2);
								tileData.minUvLeft = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
								tileData.minUvRight = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
								tileData.minUvUp = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
								tileData.minUvDown = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
								tileData.minUvMask = TileConfig::getMinUvMaskFromTileMask(mask);

								tileData.maskSide = Vec4(0, 0, 0, 0);
								tileData.randomBits = getRandomBits();
								tileData.corners = 0;

								tile->type2 = type2;
								tile->typeLeft = TileConfig::TILE_EMPTY;
								tile->typeRight = TileConfig::TILE_EMPTY;
								tile->typeUp = TileConfig::TILE_EMPTY;
								tile->typeDown = TileConfig::TILE_EMPTY;
								tile->mask = mask;
								tile->maskSide = tileData.maskSide;
								tile->randomBits = tileData.randomBits;
								tile->corners = 0;
							}
							else
							{
								tileData.minUv2 = TileConfig::getMinUvFromTileType(tile->type2);
								tileData.minUvLeft = TileConfig::getMinUvFromTileType(tile->typeLeft);
								tileData.minUvRight = TileConfig::getMinUvFromTileType(tile->typeRight);
								tileData.minUvUp = TileConfig::getMinUvFromTileType(tile->typeUp);
								tileData.minUvDown = TileConfig::getMinUvFromTileType(tile->typeDown);
								tileData.minUvMask = TileConfig::getMinUvMaskFromTileMask(tile->mask);
								tileData.maskSide = tile->maskSide;
								tileData.randomBits = tile->randomBits;
								tileData.corners = tile->corners;
							}
						}

						this->minUvs[i].push_back(tileData);
						this->translations[i].push_back(tile->pos*TILE_SIZE);
					}
				}
			}
		}

	if (calcDetail)
	{
		for (unsigned int i = 0; i < TILE_LAYERS; i++)
		{
			for (unsigned int j = 0; j < this->minUvs[i].size(); j++)
			{
				TileDrawData& tileData = this->minUvs[i][j];
				const Vec2 pos = this->translations[i][j]/TILE_SIZE;
				float posX = floor(pos.x);
				float posY = floor(pos.y);
				Tile* tile = getTileFromPos(posX, posY, i);

				if (!useWireframe)
				{
					TileConfig::TILE_TYPE typeLeft = TileConfig::TILE_EMPTY;
					TileConfig::TILE_TYPE typeRight = TileConfig::TILE_EMPTY;
					TileConfig::TILE_TYPE typeUp = TileConfig::TILE_EMPTY;
					TileConfig::TILE_TYPE typeDown = TileConfig::TILE_EMPTY;
					Vec4 maskSide(0.0f, 0.0f, 0.0f, 0.0f);
					unsigned int corners = 0;
					calculateDetail(tile->type, typeLeft, typeRight, typeUp, typeDown, maskSide, corners, posX, posY, i);
					tile->typeLeft = typeLeft;
					tile->typeRight = typeRight;
					tile->typeUp = typeUp;
					tile->typeDown = typeDown;
					tile->maskSide = maskSide;
					tile->corners = corners;

					tileData.minUvLeft = TileConfig::getMinUvFromTileType(tile->typeLeft);
					tileData.minUvRight = TileConfig::getMinUvFromTileType(tile->typeRight);
					tileData.minUvUp = TileConfig::getMinUvFromTileType(tile->typeUp);
					tileData.minUvDown = TileConfig::getMinUvFromTileType(tile->typeDown);
					tileData.maskSide = tile->maskSide;
					tileData.corners = tile->corners;
				}

			}
		}
	}
	if(this->recalculateMask)
		this->recalculateMask = false;
	*/
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
		std::cout << "Cam pos: " << Utils::toString(camPos) << std::endl;
		std::cout << "Mouse pos: " << Utils::toString(mouseOffset) << std::endl;
		tilePos.x = round(tilePos.x);
		tilePos.y = round(tilePos.y);

		const int rightBoundH = (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
		const int leftBoundH = -rightBoundH;
		const int rightBoundV = (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
		const int leftBoundV = -rightBoundV;

		if (tilePos.x > leftBoundH && tilePos.x < rightBoundH &&
			tilePos.y > leftBoundV && tilePos.y < rightBoundV)
		{
			Tile* tile = getTileFromPos(tilePos.x, tilePos.y, MIDDLE_TILE);
			if (tile != nullptr)
			{
				std::cout << "Tile pos: " << Utils::toString(tilePos) << std::endl << std::endl;// << ", type: " << TileConfig::getStrFromType(tile->type) << ", mask: " << TileConfig::getStrFromMask(tile->mask) << std::endl << std::endl;

				// Delete tile
				tile->minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
				tile->minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);
				// Update the surounding tiles.
				this->recalculateMask = true;
				/*
				// Cross tiles
				Tile* left = getTileFromPos(tilePos.x - 1, tilePos.y, MIDDLE_TILE);
				if (TILE_EXIST(left)) left->mask = TileConfig::MASK_EMPTY;
				Tile* right = getTileFromPos(tilePos.x + 1, tilePos.y, MIDDLE_TILE);
				if (TILE_EXIST(right)) right->mask = TileConfig::MASK_EMPTY;
				Tile* up = getTileFromPos(tilePos.x, tilePos.y + 1, MIDDLE_TILE);
				if (TILE_EXIST(up)) up->mask = TileConfig::MASK_EMPTY;
				Tile* down = getTileFromPos(tilePos.x, tilePos.y - 1, MIDDLE_TILE);
				if (TILE_EXIST(down)) down->mask = TileConfig::MASK_EMPTY;
				// Corner tiles
				Tile* tl = getTileFromPos(tilePos.x - 1, tilePos.y + 1, MIDDLE_TILE);
				if (TILE_EXIST(tl)) tl->mask = TileConfig::MASK_EMPTY;
				Tile* tr = getTileFromPos(tilePos.x + 1, tilePos.y + 1, MIDDLE_TILE);
				if (TILE_EXIST(tr)) tr->mask = TileConfig::MASK_EMPTY;
				Tile* br = getTileFromPos(tilePos.x + 1, tilePos.y - 1, MIDDLE_TILE);
				if (TILE_EXIST(br)) br->mask = TileConfig::MASK_EMPTY;
				Tile* bl = getTileFromPos(tilePos.x - 1, tilePos.y - 1, MIDDLE_TILE);
				if (TILE_EXIST(bl)) bl->mask = TileConfig::MASK_EMPTY;
				*/
			}
		}

	}
}

void Terrain::calculateMaskAndType(Vec2& minUv2, Vec2& minUvMask, float x, float y, unsigned int layer)
{
	Tile* left = getTileFromPos(x - 1, y, layer);
	Tile* right = getTileFromPos(x + 1, y, layer);
	Tile* up = getTileFromPos(x, y + 1, layer);
	Tile* down = getTileFromPos(x, y - 1, layer);
	if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);
		minUv2 = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
		return;
	}
	// TOP
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_T);
		minUv2 = down->minUv;
		return;
	}
	// TOP LEFT
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TL);
		minUv2 = down->minUv;
		return;
	}
	// TOP RIGHT
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TR);
		minUv2 = down->minUv;
		return;
	}
	// RIGHT
	else if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_R);
		minUv2 = left->minUv;
		return;
	}
	// BOTTOM RIGHT
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_BR);
		minUv2 = left->minUv;
		return;
	}
	// BOTTOM
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_B);
		minUv2 = up->minUv;
		return;
	}
	// BOTTOM LEFT
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_BL);
		minUv2 = up->minUv;
		return;
	}
	// LEFT
	else if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_L);
		minUv2 = right->minUv;
		return;
	}
	// SOLO TOP
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_SOLO_T);
		minUv2 = down->minUv;
		return;
	}
	// SOLO RIGHT
	else if (TILE_GONE(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_SOLO_R);
		minUv2 = left->minUv;
		return;
	}
	// SOLO BOTTOM
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_SOLO_B);
		minUv2 = up->minUv;
		return;
	}
	// SOLO LEFT
	else if (TILE_GONE(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_SOLO_L);
		minUv2 = right->minUv;
		return;
	}
	// SOLO
	else if (TILE_GONE(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_SOLO);
		minUv2 = TileConfig::getMinUvFromTileType(TileConfig::TILE_TYPE::TILE_EMPTY);
		return;
	}
	// TUBE BLOCKED V
	else if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TUBE_BLOCKED_V);
		minUv2 = down->minUv; // TODO: CHANGE THIS TO WORK FOR TWO TYPES.
		return;
	}
	// TUBE BLOCKED H
	else if (TILE_GONE(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TUBE_BLOCKED_H);
		minUv2 = left->minUv; // TODO: CHANGE THIS TO WORK FOR TWO TYPES.
		return;
	}
	// TUBE V
	else if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TUBE_V);
		minUv2 = down->minUv;
		return;
	}
	// TUBE H
	else if (TILE_GONE(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TUBE_H);
		minUv2 = left->minUv;
		return;
	}
	// TODO: Add the rest (when many types of tiles can be beside each other)

	minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_FULL);
	minUv2 = TileConfig::getMinUvFromTileType(TileConfig::TILE_TYPE::TILE_WIRE_FRAME);
	return;
}

void Terrain::calculateDetail(Vec2 minUv, Vec2& minUvLeft, Vec2& minUvRight, Vec2& minUvUp, Vec2& minUvDown, Vec4& maskSide, unsigned int& corners, float x, float y, unsigned int layer)
{
	Tile* left = getTileFromPos(x - 1, y, layer);
	Tile* right = getTileFromPos(x + 1, y, layer);
	Tile* up = getTileFromPos(x, y + 1, layer);
	Tile* down = getTileFromPos(x, y - 1, layer);

	static const Vec2 TILE_UV_EMPTY = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
	static const Vec2 MASK_UV_PATCH_FULL = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);

	if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		if (minUv != left->minUv && left->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.x = 1.0;
			minUvLeft = left->minUv;
		}
		else minUvLeft = TILE_UV_EMPTY;

		if (minUv != right->minUv && right->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.y = 1.0;
			minUvRight = right->minUv;
		}
		else minUvRight = TILE_UV_EMPTY;

		if (minUv != up->minUv && up->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.z = 1.0;
			minUvUp = up->minUv;
		}
		else minUvUp = TILE_UV_EMPTY;

		if (minUv != down->minUv && down->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.w = 1.0;
			minUvDown = down->minUv;
		}
		else minUvDown = TILE_UV_EMPTY;

		// TL
		if (minUv != left->minUv && left->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != up->minUv && up->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 0);
			minUvLeft = left->minUv; // Color of tl corner
		}

		if (minUv != right->minUv && right->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != up->minUv && up->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 1);
			minUvRight = up->minUv; // Color of tr corner
		}

		if (minUv != right->minUv && right->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 2);
			minUvUp = right->minUv; // Color of br corner
		}

		if (minUv != left->minUv && left->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 3);
			minUvDown = down->minUv; // Color of bl corner
		}
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_GONE(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;
		return;
	}
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

	return &this->chunks[v][h]->tiles[layer][yc][xc];
}

unsigned int Terrain::getRandomBits() const
{
	unsigned int bits = this->randomNumber; // Will be the same the entire game.
	// Corner TL 
	Utils::setBit<unsigned int>(bits, 0);
	Utils::setBit<unsigned int>(bits, 1);
	Utils::setBit<unsigned int>(bits, 27);

	// Corner TR 
	Utils::setBit<unsigned int>(bits, 6);
	Utils::setBit<unsigned int>(bits, 7);
	Utils::setBit<unsigned int>(bits, 8);

	// Corner BR 
	Utils::setBit<unsigned int>(bits, 13);
	Utils::setBit<unsigned int>(bits, 14);
	Utils::setBit<unsigned int>(bits, 15);

	// Corner BL 
	Utils::setBit<unsigned int>(bits, 20);
	Utils::setBit<unsigned int>(bits, 21);
	Utils::setBit<unsigned int>(bits, 22);
	/*
	// TL Side bits
	setBitsIfNot(bits, 2, 3);
	setBitsIfNot(bits, 26, 25);

	// TR Side bits
	setBitsIfNot(bits, 4, 5);
	setBitsIfNot(bits, 9, 10);

	// BR Side bits
	setBitsIfNot(bits, 11, 12);
	setBitsIfNot(bits, 16, 17);

	// BL Side bits
	setBitsIfNot(bits, 19, 18);
	setBitsIfNot(bits, 24, 23);
	*/
	return bits;
}

void Terrain::setBitsIfNot(unsigned int& bits, unsigned int bit1, unsigned int bit2) const
{
	bool x = Utils::isBitSet<unsigned int>(bits, bit1);
	bool y = Utils::isBitSet<unsigned int>(bits, bit2);
	if (!x && !y) Utils::setBit<unsigned int>(bits, rand() % 2 + bit1);
}
