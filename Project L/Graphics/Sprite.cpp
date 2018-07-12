#include "Sprite.h"

#include "../Rendering/ModelManager.h"
#include "../Maths/MathsTransform.h"

Sprite::Sprite() : position(Vec2(0.0f, 0.0f)), scale(Vec2(1.0f, 1.0f)), tint(Vec3(1.0f, 1.0f, 1.0f)), angle(0.0f), texture(nullptr)
{
	this->model = ModelManager::get("RectangleModel");
}

Sprite::Sprite(Texture * texture, const Vec2 & position) : position(position), scale(Vec2(1.0f, 1.0f)), tint(Vec3(1.0f, 1.0f, 1.0f)), angle(0.0f)
{
	this->texture = texture;
	this->model = ModelManager::get("RectangleModel");
}

void Sprite::setTexture(Texture * texture)
{
	this->texture = texture;
}

const Texture* Sprite::getTexture() const
{
	return this->texture;
}

Model * Sprite::getModel() const
{
	return this->model;
}

Vec3 Sprite::getTint() const
{
	return this->tint;
}

Vec2 Sprite::getScale() const
{
	return this->scale;
}

Vec2 Sprite::getPosition() const
{
	return this->position;
}

float Sprite::getAngle() const
{
	return this->angle;
}

void Sprite::setTint(const Vec3 & tint)
{
	this->tint = tint;
}

void Sprite::setScale(const Vec2 & scale)
{
	this->scale = scale;
}

void Sprite::setPosition(const Vec2 & position)
{
	this->position = position;
}

void Sprite::setAngle(float angle)
{
	this->angle = angle;
}

Mat3 Sprite::getMatrix() const
{
	Mat3 scale(this->scale.x, 0.0f, 0.0f,
			   0.0f, this->scale.y, 0.0f,
			   0.0f, 0.0f, 1.0f);
	Mat3 position(1.0f, 0.0f, this->position.x,
				0.0f, 1.0f, this->position.y,
				0.0f, 0.0f, 1.0f);
	Mat3 rotation = MathsTransform::rotation(this->angle);

	return position*scale*rotation;
}
