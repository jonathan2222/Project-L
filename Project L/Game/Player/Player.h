#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "../../GUI/Display.h"

class Terrain;
class Player
{
public:
	Player();
	~Player();

	void processInput(Display* display, Terrain* terrain);

	Camera& getCamera();

private:
	void processScrolling();
	void processDraging(Display* display, const Vec2& mousePosition);

	Camera camera;
};

#endif
