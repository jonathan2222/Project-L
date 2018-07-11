#include "Player.h"

#include "../Terrain/Terrain.h"
#include "../../Utils/Error.h"
#include "../../Input/Input.h"

Player::Player()
{
	this->camera.setZoom(28.0f);
	this->selectedTile = TileConfig::TILE_TYPE::TILE_DIRT;
}

Player::~Player()
{
}

void Player::update(float dt, Display* display, Terrain* terrain)
{
	processInput(display, terrain);
	processControls(dt);
}

Camera & Player::getCamera()
{
	return this->camera;
}

Vec4 Player::getTileIndexInFocus() const
{
	return this->tileIndexInFocus;
}

TileConfig::TILE_TYPE Player::getSelectedTile() const
{
	return this->selectedTile;
}

void Player::processInput(Display* display, Terrain* terrain)
{
	processScrolling();

	Vec2 mPos = Input::getMousePosition();
	processDraging(display, mPos, GLFW_MOUSE_BUTTON_MIDDLE);

	// Cursor tile.
	Vec2 tilePos = getTilePosFromMousePos(display, mPos);
	if (terrain->isPosInsideTerrain(tilePos.x, tilePos.y))
	{
		static Tile* preTile = nullptr;
		static Vec2 preChunkPos;
		static Vec2 preIndexInChunk(-1.0f, -1.0f);
		this->tileIndexInFocus = terrain->getChunkIndicesFromPos(tilePos.x, tilePos.y);
		if (tileIndexInFocus.x != -1.0f)
		{
			if (preTile != nullptr && preChunkPos != Vec2(tileIndexInFocus.x, tileIndexInFocus.y))
			{
				preTile->pos = { 0.0f, 0.0f };
				terrain->removeTile((unsigned int)preChunkPos.x, (unsigned int)preChunkPos.y, 0, 0, FRONT_TILE);
				// Update the previous chunk.
				terrain->getChunk((unsigned int)preChunkPos.x, (unsigned int)preChunkPos.y)->updateLayer(FRONT_TILE);
			}
			Tile* tileFront = terrain->getTile((unsigned int)tileIndexInFocus.x, (unsigned int)tileIndexInFocus.y, 0, 0, FRONT_TILE);
			
			if (tileFront != nullptr)
			{
				tileFront->pos = { tilePos.x, tilePos.y };
				tileFront->minUv = TileConfig::getMinUvFromTileType(TileConfig::TILE_WIRE_FRAME);
				tileFront->minUvMask = TileConfig::getMinUvMaskFromTileMask(TileConfig::MASK_PATCH_FULL);

				// Update the chunk.
				if (preIndexInChunk != Vec2(tileIndexInFocus.z, tileIndexInFocus.w))
					terrain->getChunk((unsigned int)tileIndexInFocus.x, (unsigned int)tileIndexInFocus.y)->updateLayer(FRONT_TILE);

				preChunkPos = { tileIndexInFocus.x, tileIndexInFocus.y };
				preIndexInChunk = { tileIndexInFocus.z, tileIndexInFocus.w };
				preTile = tileFront;
			}
		}
	}

	if (Input::isKeyClicked(GLFW_KEY_Q)) this->selectedTile = (TileConfig::TILE_TYPE)(this->selectedTile - 1);
	if (Input::isKeyClicked(GLFW_KEY_E)) this->selectedTile = (TileConfig::TILE_TYPE)(this->selectedTile + 1);
	if (this->selectedTile > TileConfig::TILE_TYPE::MAX_NUM_TYPES - 1)
		this->selectedTile = (TileConfig::TILE_TYPE)3;
	else if (this->selectedTile < 3) // Skip empty, wireframe and sky tiles.
		this->selectedTile = (TileConfig::TILE_TYPE)(TileConfig::TILE_TYPE::MAX_NUM_TYPES - 1);

	// If LMB is pressed, place the selected tile type on the tile which the mouse is over.
	if (Input::isButtonClicked(GLFW_MOUSE_BUTTON_LEFT))
	{
		// Place tile
		if (terrain->setTile(this->selectedTile, this->selectedTile == TileConfig::TILE_GRASS ? TileConfig::FLAG_GROW_TYPE : 0, tilePos.x, tilePos.y, MIDDLE_TILE))
		{
			//Tile* tile = terrain->getTileFromPos(tilePos.x, tilePos.y, MIDDLE_TILE);
			//tile->pos.x += 4.0f * TILE_SIZE/TILE_IMG_SIZE;
			terrain->updateVisibleChunks();
		}
	}

	// If RMB is pressed, delete the tile which the mouse is over.
	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		// Delete tile
		if (terrain->removeTile(tilePos.x, tilePos.y, MIDDLE_TILE))
			terrain->updateVisibleChunks();
	}

	this->camera.setZoom(this->camera.getZoom(), display->getRatio());
}

void Player::processScrolling()
{
	if (Input::isScrolling())
	{
		float newZoom = this->camera.getZoom() + TILE_SIZE * -Input::getScrollYOffest();
		if (newZoom > 0)
			this->camera.setZoom(newZoom);
		Error::printWarning("Zoom: " + std::to_string(newZoom));
	}
}

void Player::processDraging(Display* display, const Vec2 & mousePosition, int button)
{
	static Vec2 prePos(-1, -1);
	static Vec2 dist(0.0, 0.0);
	if (Input::isButtonPressed(button))
	{
		if (prePos.x == -1 && prePos.y == -1)
			prePos = mousePosition;

		dist = mousePosition - prePos;
		Vec3 translation;
		translation.x = -(float)dist.x / display->getWidth() * this->camera.getZoom() * display->getRatio();
		translation.y = (float)dist.y / display->getHeight() * this->camera.getZoom();
		this->camera.move(translation);

		prePos = mousePosition;
	}
	else
	{
		prePos.x = -1;
		prePos.y = -1;
	}
}

void Player::processControls(float dt)
{
	// Fix this to be consistent.
	static const float SPEED = 10.0f;
	float adjustedSpeed = TILE_SIZE * dt;
	if (Input::isKeyPressed(GLFW_KEY_A))
		this->camera.move({ -SPEED * adjustedSpeed, 0.0f, 0.0f });
	if (Input::isKeyPressed(GLFW_KEY_D))
		this->camera.move({ SPEED * adjustedSpeed, 0.0f, 0.0f });
	if (Input::isKeyPressed(GLFW_KEY_W))
		this->camera.move({ 0.0f, SPEED * adjustedSpeed, 0.0f });
	if (Input::isKeyPressed(GLFW_KEY_S))
		this->camera.move({ 0.0f, -SPEED * adjustedSpeed, 0.0f });
}

Vec2 Player::getTilePosFromMousePos(Display * display, const Vec2 & mousePosition)
{
	const Vec3 camPos = this->camera.getPosition() / TILE_SIZE;
	const float numTilesX = this->camera.getZoom()*display->getRatio() / TILE_SIZE;
	const float numTilesY = this->camera.getZoom() / TILE_SIZE;

	Vec2 mouseOffset((mousePosition.x / display->getWidth() - 0.5f) * numTilesX, 
					 (mousePosition.y / display->getHeight() - 0.5f) * numTilesY);
	mouseOffset.y = -mouseOffset.y;

	Vec2 tilePos = camPos + mouseOffset;
	tilePos.y = round(tilePos.y);
	tilePos.x = round(tilePos.x);
	return tilePos;
}
