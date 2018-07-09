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

#include "../../Input/Input.h"

Terrain::Terrain()
{
	this->terrainShader = new Shader("Shaders/Terrain/terrainInstanced.fs", "Shaders/Terrain/terrainInstanced.vs");

	ResourceManager::addTexture("Tile Map", "Textures/Terrain/tile_map_8x8.png"); // Map can hold 1024 different 32x32 tiles or 4096 16x16 tiles.

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
					int padding = (int)floor(noise*CHUNK_SIZE*0.7f);
					noise = floor((noise*2.0f - 1.0f)*NUM_CHUNKS_VERTICAL*CHUNK_SIZE*0.25f);
					if (noise < tile.pos.y) tile.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_EMPTY);
					if (noise == tile.pos.y)
					{
						tile.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_GRASS);
						tile.flags = TileConfig::FLAG_GROW_TYPE;
					}
					else if (noise > tile.pos.y) tile.minUv =TileConfig::getMinUvFromTileType(TileConfig::TILE_DIRT);
					if (noise- padding > tile.pos.y) tile.minUv = TileConfig::getMinUvFromTileType(rand()%5 == 0 ? (rand() % 5 == 0 ? TileConfig::TILE_STONE_GOLD : (rand()%4 == 0 ?TileConfig::TILE_STONE_2 : TileConfig::TILE_STONE_3)) : TileConfig::TILE_STONE);
					//tile.minUv = TileConfig::getMinUvFromTileType((TileConfig::TILE_TYPE)((xc+yc)%((int)TileConfig::MAX_NUM_TYPES - 3) + 3));
					//tile.minUv = TileConfig::getMinUvFromTileType((TileConfig::TILE_TYPE)((v+h+xc + yc) % (int)TileConfig::MAX_NUM_TYPES));

					tile.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);// TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY); // Init mask to empty.
					
					Tile& background = chunk.tiles[BACK_TILE][yc][xc];
					background.pos = tile.pos;
					background.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_SKY);
					background.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);
					
					/*
					Tile& foreground = chunk.tiles[FRONT_TILE][yc][xc];
					foreground.pos = tile.pos;
					foreground.minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_WIRE_FRAME);
					foreground.minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_EMPTY);
					*/
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

void Terrain::draw(Camera& camera, const Renderer & renderer, Display* display)
{

	// Get the cunks to be drawn.
	this->chunksToDraw.clear();
	getVisibleChunks(camera, display);

	// Add a mask and detail to each tile in the visible chunks.
	if (calculateMask())
	{
		calculateDetail();
		for (const std::pair<unsigned int, unsigned int>& chunkIndex : this->chunksToDraw)
			this->chunks[chunkIndex.first][chunkIndex.second]->updateLayers();
	}
	this->recalculateMask = false;
	
	static bool useWireframe = false;
	if (Input::isKeyClicked(GLFW_KEY_F))
		useWireframe ^= 1;
	for (const std::pair<unsigned int, unsigned int>& chunkIndex : this->chunksToDraw)
		drawChunk(camera, this->chunks[chunkIndex.first][chunkIndex.second], renderer, useWireframe);
}

void Terrain::drawChunk(Camera& camera, Chunk* chunk, const Renderer& renderer, bool useWireframe)
{
	this->terrainShader->bind();
	this->terrainShader->setUniformMatrix4fv("camera", 1, false, &camera.getMatrix()[0][0]);
	this->terrainShader->setUniform1i("tileSize", TILE_IMG_SIZE);
	this->terrainShader->setUniformMatrix3fv("transform", 1, false, &(this->transform[0][0]));
	this->terrainShader->setTexture2D("tex", 0, ResourceManager::getTexture("Tile Map")->getID());
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
		chunk->draw(renderer, i, useWireframe);
}

void Terrain::getVisibleChunks(Camera& camera, Display* display)
{
	const Vec3 camPos = camera.getPosition() / TILE_SIZE;
	float numTilesX = ceil(camera.getZoom()*display->getRatio() / TILE_SIZE + 1.0f);
	float numTilesY = ceil(camera.getZoom() / TILE_SIZE + 1.0f);

	Vec4 chunkIndices = getChunkIndicesFromPos(camPos.x, camPos.y);
	if (chunkIndices.x != -1)
	{
		Vec2 chunkIndex(chunkIndices.x, chunkIndices.y);
		Vec2 indexInChunk(chunkIndices.z, chunkIndices.w);
		indexInChunk.x = CHUNK_SIZE / 2 - abs(indexInChunk.x + 1 - CHUNK_SIZE / 2);
		indexInChunk.y = CHUNK_SIZE / 2 - abs(indexInChunk.y + 1 - CHUNK_SIZE / 2);

		float sideTilesX = ceil(numTilesX / 2);
		float sideTilesY = ceil(numTilesY / 2);

		float nHalfChunksH = indexInChunk.x < sideTilesX ? ceil((sideTilesX - indexInChunk.x) / CHUNK_SIZE) : 0;
		float nHalfChunksV = indexInChunk.y < sideTilesY ? ceil((sideTilesY - indexInChunk.y) / CHUNK_SIZE) : 0;

		if (nHalfChunksH == 0 || nHalfChunksV == 0)
			this->chunksToDraw.push_back({ (unsigned int)chunkIndices.x, (unsigned int)chunkIndices.y });
		for (float i = -nHalfChunksH; i <= nHalfChunksH; i++)
			for (float j = -nHalfChunksV; j <= nHalfChunksV; j++)
			{
				unsigned int v = (unsigned int)(chunkIndex.x + j);
				unsigned int h = (unsigned int)(chunkIndex.y + i);
				if (v < NUM_CHUNKS_VERTICAL && v >= 0 && h < NUM_CHUNKS_HORIZONTAL && h >= 0)
					this->chunksToDraw.push_back({ v, h });
			}
	}
}

bool Terrain::calculateMask()
{
	bool shouldCalculateDetail = false;
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
					if (tile.minUv != TileConfig::TILE_UV_EMPTY)
					{
						if ((tile.minUvMask == TileConfig::MASK_UV_EMPTY) || this->recalculateMask)
						{
							shouldCalculateDetail = true;
							Vec2 minUvMask;
							Vec2 minUv2 = TileConfig::TILE_UV_EMPTY;

							calculateMask(tile.flags, tile.minUv, minUv2, minUvMask, tile.pos.x, tile.pos.y, i);

							tile.minUv2 = minUv2;
							tile.minUvLeft = TileConfig::TILE_UV_EMPTY;
							tile.minUvRight = TileConfig::TILE_UV_EMPTY;
							tile.minUvUp = TileConfig::TILE_UV_EMPTY;
							tile.minUvDown = TileConfig::TILE_UV_EMPTY;
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
	return shouldCalculateDetail;
}

void Terrain::calculateMask(unsigned int flags, Vec2 minUv, Vec2& minUv2, Vec2& minUvMask, float x, float y, unsigned int layer)
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
		if ((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = down->minUv;
		return;
	}
	// TOP LEFT
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TL);
		if ((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = down->minUv;
		return;
	}
	// TOP RIGHT
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TR);
		if ((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = down->minUv;
		return;
	}
	// RIGHT
	else if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_R);
		if ((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = left->minUv;
		return;
	}
	// BOTTOM RIGHT
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_BR);
		if ((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = left->minUv;
		return;
	}
	// BOTTOM
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_B);
		if((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = up->minUv;
		return;
	}
	// BOTTOM LEFT
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_BL);
		if ((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = up->minUv;
		return;
	}
	// LEFT
	else if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_L);
		if ((flags & TileConfig::FLAG_GROW_TYPE) == 0) minUv2 = minUv; else minUv2 = right->minUv;
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
	else if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_GONE(left) && TILE_GONE(right) && up->minUv != down->minUv)
	{
		minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::TILE_MASK::MASK_PATCH_TUBE_BLOCKED_V);
		minUv2 = down->minUv; // TODO: CHANGE THIS TO WORK FOR TWO TYPES.
		return;
	}
	// TUBE BLOCKED H
	else if (TILE_GONE(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_EXIST(right) && left->minUv != right->minUv)
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

void Terrain::calculateDetail()
{
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
					if (tile.minUv != TileConfig::TILE_UV_EMPTY)
					{
						Vec2 minUvLeft = TileConfig::TILE_UV_EMPTY;
						Vec2 minUvRight = TileConfig::TILE_UV_EMPTY;
						Vec2 minUvUp = TileConfig::TILE_UV_EMPTY;
						Vec2 minUvDown = TileConfig::TILE_UV_EMPTY;
						Vec4 maskSide;
						unsigned int corners = 0;

						calculateDetail(tile.flags, tile.minUv, minUvLeft, minUvRight, minUvUp, minUvDown, maskSide, corners, tile.pos.x, tile.pos.y, i);

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
}

void Terrain::calculateDetail(unsigned int flags, Vec2 minUv, Vec2& minUvLeft, Vec2& minUvRight, Vec2& minUvUp, Vec2& minUvDown, Vec4& maskSide, unsigned int& corners, float x, float y, unsigned int layer)
{
	Tile* left = getTileFromPos(x - 1, y, layer);
	Tile* right = getTileFromPos(x + 1, y, layer);
	Tile* up = getTileFromPos(x, y + 1, layer);
	Tile* down = getTileFromPos(x, y - 1, layer);

	static const Vec2 MASK_UV_PATCH_FULL = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);

	if (TILE_EXIST(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		if (minUv != left->minUv && left->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.x = 1.0;
			minUvLeft = left->minUv;
		}
		else minUvLeft = TileConfig::TILE_UV_EMPTY;

		if (minUv != right->minUv && right->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.y = 1.0;
			minUvRight = right->minUv;
		}
		else minUvRight = TileConfig::TILE_UV_EMPTY;

		if (minUv != up->minUv && up->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.z = 1.0;
			minUvUp = up->minUv;
		}
		else minUvUp = TileConfig::TILE_UV_EMPTY;

		if (minUv != down->minUv && down->minUvMask == MASK_UV_PATCH_FULL)
		{
			maskSide.w = 1.0;
			minUvDown = down->minUv;
		}
		else minUvDown = TileConfig::TILE_UV_EMPTY;

		// TL
		if (minUv != left->minUv && left->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != up->minUv && up->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 0);
			minUvLeft = left->minUv; // Color of tl corner
		}

		// TR
		if (minUv != right->minUv && right->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != up->minUv && up->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 1);
			minUvRight = up->minUv; // Color of tr corner
		}

		// BR
		if (minUv != right->minUv && right->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 2);
			minUvUp = right->minUv; // Color of br corner
		}

		// BL
		if (minUv != left->minUv && left->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 3);
			minUvDown = down->minUv; // Color of bl corner
		}
		return;
	}
	/*
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;

		// TR
		if (minUv != right->minUv && right->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != up->minUv && up->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 1);
			minUvRight = up->minUv; // Color of tr corner
		}
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;

		// BL
		if (minUv != left->minUv && left->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 3);
			minUvDown = down->minUv; // Color of bl corner
		}
		return;
	}
	else if (TILE_GONE(down) && TILE_EXIST(up) && TILE_EXIST(left) && TILE_GONE(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;

		// TL
		if (minUv != left->minUv && left->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != up->minUv && up->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 0);
			minUvLeft = left->minUv; // Color of tl corner
		}
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_GONE(left) && TILE_EXIST(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;

		// BR
		if (minUv != right->minUv && right->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 2);
			minUvUp = right->minUv; // Color of br corner
		}
		return;
	}
	else if (TILE_EXIST(down) && TILE_GONE(up) && TILE_EXIST(left) && TILE_EXIST(right))
	{
		minUvLeft = TILE_UV_EMPTY;
		minUvRight = TILE_UV_EMPTY;
		minUvUp = TILE_UV_EMPTY;
		minUvDown = TILE_UV_EMPTY;

		// BR
		if (minUv != right->minUv && right->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 2);
			minUvUp = right->minUv; // Color of br corner
		}

		// BL
		if (minUv != left->minUv && left->minUvMask != MASK_UV_PATCH_FULL &&
			minUv != down->minUv && down->minUvMask != MASK_UV_PATCH_FULL)
		{
			Utils::setBit<unsigned int>(corners, 3);
			minUvDown = down->minUv; // Color of bl corner
		}
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
	*/
}

Vec4 Terrain::getChunkIndicesFromPos(float x, float y)
{
	const float posXT = x + (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;
	const float posYT = y + (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE / 2;

	const int h = (int)floor(posXT / CHUNK_SIZE);
	const int v = (int)floor(posYT / CHUNK_SIZE);

	const int xc = (int)posXT % CHUNK_SIZE;
	const int yc = (int)posYT % CHUNK_SIZE;

	if (!(v < NUM_CHUNKS_VERTICAL && h < NUM_CHUNKS_HORIZONTAL && v >= 0 && h >= 0))
		return Vec4(-1.0f, -1.0f, -1.0f, -1.0f);
	if (!(xc < CHUNK_SIZE && yc < CHUNK_SIZE && xc >= 0 && yc >= 0))
		return Vec4(-1.0f, -1.0f, -1.0f, -1.0f);

	return Vec4((float)v, (float)h, (float)xc, (float)yc);
}

Tile* Terrain::getTileFromPos(float x, float y, unsigned int layer)
{
	Vec4 chunkIndices = getChunkIndicesFromPos(x, y);
	if (chunkIndices.x == -1.0f && chunkIndices.y == -1.0f && chunkIndices.z == -1.0f && chunkIndices.w == -1.0f)
		return nullptr;

	const unsigned int v = (unsigned int)chunkIndices.x;
	const unsigned int h = (unsigned int)chunkIndices.y;
	const unsigned int xc = (unsigned int)chunkIndices.z;
	const unsigned int yc = (unsigned int)chunkIndices.w;

	return &this->chunks[v][h]->tiles[layer][yc][xc];
}

Chunk * Terrain::getChunk(unsigned int v, unsigned int h)
{
	return this->chunks[v][h];
}

Tile * Terrain::getTile(unsigned int v, unsigned int h, unsigned int x, unsigned int y, unsigned int layer)
{
	return &this->chunks[v][h]->tiles[layer][y][x];
}

bool Terrain::removeTile(unsigned int v, unsigned int h, unsigned int x, unsigned int y, unsigned int layer)
{
	Tile* tile = getTile(v, h, x, y, layer);
	if (tile != nullptr)
	{
		tile->minUv = TileConfig::TILE_UV_EMPTY;
		tile->minUvMask = TileConfig::MASK_UV_EMPTY;
		return true;
	}
	return false;
}

bool Terrain::removeTile(float x, float y, unsigned int layer)
{
	Vec4 chunkIndices = getChunkIndicesFromPos(x, y);
	if (chunkIndices.x == -1.0f && chunkIndices.y == -1.0f && chunkIndices.z == -1.0f && chunkIndices.w == -1.0f)
		return false;

	const unsigned int v = (unsigned int)chunkIndices.x;
	const unsigned int h = (unsigned int)chunkIndices.y;
	const unsigned int xc = (unsigned int)chunkIndices.z;
	const unsigned int yc = (unsigned int)chunkIndices.w;

	Tile& tile = this->chunks[v][h]->tiles[layer][yc][xc];
	tile.minUv = TileConfig::TILE_UV_EMPTY;
	tile.minUvMask = TileConfig::MASK_UV_EMPTY;
	return true;
}

bool Terrain::isPosInsideTerrain(float x, float y) const
{
	const int rightBoundH = (int)(NUM_CHUNKS_HORIZONTAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
	const int leftBoundH = -rightBoundH;
	const int rightBoundV = (int)(NUM_CHUNKS_VERTICAL / 2)*CHUNK_SIZE + CHUNK_SIZE;
	const int leftBoundV = -rightBoundV;

	return x > leftBoundH && x < rightBoundH && y > leftBoundV && y < rightBoundV;
}

void Terrain::updateVisibleChunks()
{
	this->recalculateMask = true;
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
	return bits;
}
