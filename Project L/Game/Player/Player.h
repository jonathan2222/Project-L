#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "../../GUI/Display.h"
#include "../Terrain/TileConfig.h"
#include "../../Graphics/Sprite.h"

#include "Box2D\Box2D.h"

class Tile;
class Terrain;
class Player
{
public:
	Player(b2World* world);
	~Player();

	void update(float dt, Display* display, Terrain* terrain);

	Camera& getCamera();

	Vec4 getTileIndexInFocus() const;
	TileConfig::TILE_TYPE getSelectedTile() const;

	Sprite& getSprite();

private:
	void processInput(Display* display, Terrain* terrain);
	void processScrolling();
	void processDraging(Display* display, const Vec2& mousePosition, int button);
	void processControls(float dt);
	Vec2 getTilePosFromMousePos(Display* display, const Vec2& mousePosition);

	Camera camera;
	Vec4 tileIndexInFocus;

	Sprite sprite;
	b2Body* body;

	TileConfig::TILE_TYPE selectedTile;
};

#endif
