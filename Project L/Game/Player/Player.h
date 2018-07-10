#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "../../GUI/Display.h"
#include "../Terrain/TileConfig.h"

class Tile;
class Terrain;
class Player
{
public:
	Player();
	~Player();

	void update(float dt, Display* display, Terrain* terrain);

	Camera& getCamera();

	Vec4 getTileIndexInFocus() const;
	TileConfig::TILE_TYPE getSelectedTile() const;

private:
	void processInput(Display* display, Terrain* terrain);
	void processScrolling();
	void processDraging(Display* display, const Vec2& mousePosition, int button);
	void processControls(float dt);
	Vec2 getTilePosFromMousePos(Display* display, const Vec2& mousePosition);

	Camera camera;
	Vec4 tileIndexInFocus;

	TileConfig::TILE_TYPE selectedTile;
};

#endif
