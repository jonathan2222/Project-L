#include "Player.h"

#include "../Terrain/Terrain.h"
#include "../../Utils/Error.h"

Player::Player()
{
	camera.setZoom(28.0f);
}

Player::~Player()
{
}

void Player::processInput(Display* display, Terrain* terrain)
{
	processScrolling();

	Vec2 mPos = Input::getMousePosition();
	processDraging(display, mPos);

	if (Input::isKeyClicked(GLFW_KEY_A))
		camera.move({ -TILE_SIZE, 0.0f, 0.0f });
	if (Input::isKeyClicked(GLFW_KEY_D))
		camera.move({ TILE_SIZE, 0.0f, 0.0f });
	if (Input::isKeyClicked(GLFW_KEY_W))
		camera.move({ 0.0f, TILE_SIZE, 0.0f });
	if (Input::isKeyClicked(GLFW_KEY_S))
		camera.move({ 0.0f, -TILE_SIZE, 0.0f });

	camera.setZoom(camera.getZoom(), display->getRatio());
}

Camera & Player::getCamera()
{
	return this->camera;
}

void Player::processScrolling()
{
	if (Input::isScrolling())
	{
		float newZoom = camera.getZoom() + TILE_SIZE * -Input::getScrollYOffest();
		if (newZoom > 0)
			camera.setZoom(newZoom);
		Error::printWarning("Zoom: " + std::to_string(newZoom));
	}
}

void Player::processDraging(Display* display, const Vec2 & mousePosition)
{
	static Vec2 prePos(-1, -1);
	static Vec2 dist(0.0, 0.0);
	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (prePos.x == -1 && prePos.y == -1)
			prePos = mousePosition;

		dist = mousePosition - prePos;
		Vec3 translation;
		translation.x = -(float)dist.x / display->getWidth() * camera.getZoom() * display->getRatio();
		translation.y = (float)dist.y / display->getHeight() * camera.getZoom();
		camera.move(translation);

		prePos = mousePosition;
	}
	else
	{
		prePos.x = -1;
		prePos.y = -1;
	}
}
