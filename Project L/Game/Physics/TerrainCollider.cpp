#include "TerrainCollider.h"
#include "../Terrain/TileConfig.h"

#include "../../ResourceManager.h"

TerrainCollider::TerrainCollider(b2World* world) : terrainPtr(nullptr), worldPtr(world)
{
	ResourceManager::addTexture("GreenBorder 16x16", "Textures/GreenBorder.png");
	ResourceManager::addTexture("RedBorder 16x16", "Textures/RedBorder.png");
	this->bodyOffset.x = 0.0f;
	this->bodyOffset.y = -1.0f / TILE_IMG_SIZE;
}

TerrainCollider::~TerrainCollider()
{
}

void TerrainCollider::createBodies(const Vec2 & centerTilePos, unsigned int layer)
{
	this->preCenterTilePosClamped = this->preCenterTilePos = centerTilePos;
	static const float LEFT = -(int)(COLLISION_BOX_WIDTH / 2.0f);
	static const float TOP = -(int)(COLLISION_BOX_HEIGHT / 2.0f);
	Tile* tile = nullptr;
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.0f;
	fixture.friction = 0.9f;
	shape.SetAsBox(0.5f, 0.5f); // 1x1 box
	for (unsigned int x = 0; x < COLLISION_BOX_WIDTH; x++)
	{
		for (unsigned int y = 0; y < COLLISION_BOX_HEIGHT; y++)
		{
			const float tx = centerTilePos.x + LEFT + (float)x;
			const float ty = centerTilePos.y + TOP + (float)y;
			tile = this->terrainPtr->getTile(tx, ty, layer);
			// Create body.
			bodyDef.position.Set(tx + this->bodyOffset.x, ty + this->bodyOffset.y);
			bodies[x][y] = this->worldPtr->CreateBody(&bodyDef);
			fixture.filter.categoryBits = (tile->flags & TileConfig::FLAG_COLLIDABLE) ? BODY_FILLED : BODY_EMPTY;
			bodies[x][y]->CreateFixture(&fixture);
		}
	}
	
}

void TerrainCollider::updateBodies(const Vec2 & centerTilePos, unsigned int layer)
{
	// Update only if moved.
	this->preCenterTilePos += centerTilePos - this->preCenterTilePos;
	if (this->preCenterTilePosClamped.x != round(this->preCenterTilePos.x) || this->preCenterTilePosClamped.y != round(this->preCenterTilePos.y))
	{
		Tile* tile = nullptr;
		const float diffX = round(centerTilePos.x - this->preCenterTilePosClamped.x);
		const float diffY = round(centerTilePos.y - this->preCenterTilePosClamped.y);
		for (unsigned int x = 0; x < COLLISION_BOX_WIDTH; x++)
			for (unsigned int y = 0; y < COLLISION_BOX_HEIGHT; y++)
			{
				Vec2 tilePos(bodies[x][y]->GetPosition().x + diffX, bodies[x][y]->GetPosition().y + diffY);
				bodies[x][y]->SetTransform(b2Vec2(tilePos.x, tilePos.y), bodies[x][y]->GetAngle());

				tile = this->terrainPtr->getTile(tilePos.x - this->bodyOffset.x, tilePos.y - this->bodyOffset.y, layer);
				b2Fixture& fixture = bodies[x][y]->GetFixtureList()[0];
				b2Filter filter = fixture.GetFilterData();
				if(tile != nullptr)
					filter.categoryBits = (tile->flags & TileConfig::FLAG_COLLIDABLE) ? BODY_FILLED : BODY_EMPTY;
				fixture.SetFilterData(filter);
			}

		this->preCenterTilePosClamped.x = round(this->preCenterTilePos.x);
		this->preCenterTilePosClamped.y = round(this->preCenterTilePos.y);
	}
}

void TerrainCollider::drawBodies(const Renderer & renderer, Shader& shader, const Mat4& camera)
{
	for (unsigned int x = 0; x < COLLISION_BOX_WIDTH; x++)
		for (unsigned int y = 0; y < COLLISION_BOX_HEIGHT; y++)
		{
			bool isG = bodies[x][y]->GetFixtureList()[0].GetFilterData().categoryBits & BODY_FILLED;
			sprite.setTexture(isG ? ResourceManager::getTexture("GreenBorder 16x16") : ResourceManager::getTexture("RedBorder 16x16"));
			sprite.setPosition(Vec2(bodies[x][y]->GetPosition().x, bodies[x][y]->GetPosition().y));
			sprite.setAngle(bodies[x][y]->GetAngle());
			renderer.drawSprite(sprite, shader, camera);
		}
}

void TerrainCollider::setTerrain(Terrain * terrainPtr)
{
	this->terrainPtr = terrainPtr;
}
