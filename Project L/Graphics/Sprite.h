#ifndef SPRITE_H
#define SPRITE_H

#include "../GLAbstractions/Texture.h"
#include "../ResourceManager.h"

#include "../Maths/Vectors/Vec2.h"
#include "../Maths/Matrices/Mat3.h"
#include "../Rendering/Model.h"

class Sprite
{
public:
	Sprite(Texture* image, const Vec2& position);
	void setTexture(Texture* image);

	const Texture* getTexture() const;
	Model* getModel() const;

	Vec3 getTint() const;
	Vec2 getScale() const;
	Vec2 getPosition() const;
	float getAngle() const;

	void setTint(const Vec3& tint);
	void setScale(const Vec2& scale);
	void setPosition(const Vec2& position);
	void setAngle(float angle);

	Mat3 getMatrix() const;

private:
	Texture* texture;
	Model* model;
	Vec3 tint;
	Vec2 position;
	Vec2 scale;
	float angle;
};

#endif
