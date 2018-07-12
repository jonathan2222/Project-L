#ifndef TERRAIN_COLLIDER_H
#define TERRAIN_COLLIDER_H

#include "Box2D\Box2D.h"
#include "../Terrain/Terrain.h"

#include "../../Graphics/Sprite.h"
#include "../../Rendering/Renderer.h"
#include "../../Shaders/Shader.h"
#include "../../Maths/Matrices/Mat4.h"

// This needs to be odd.
#define COLLISION_BOX_WIDTH 7
#define COLLISION_BOX_HEIGHT 7

#define BODY_FILLED 0x0001
#define BODY_EMPTY  0x0002

class TerrainCollider
{
public:
	TerrainCollider(b2World* world);
	~TerrainCollider();

	void createBodies(const Vec2& centerTilePos, unsigned int layer);
	void updateBodies(const Vec2& centerTilePos, unsigned int layer);

	void drawBodies(const Renderer& renderer, Shader& shader, const Mat4& camera);

	void setTerrain(Terrain* terrainPtr);

private:
	b2Body * bodies[COLLISION_BOX_WIDTH][COLLISION_BOX_HEIGHT];
	Terrain* terrainPtr;
	b2World* worldPtr;
	Vec2 preCenterTilePos;
	Vec2 preCenterTilePosClamped;
	Sprite sprite;
	Vec2 bodyOffset;
};

#endif
