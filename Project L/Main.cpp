#include <iostream>

#include "GUI\Display.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"
#include "Maths\Matrices\Mat3.h"
#include "Maths\Matrices\Mat4.h"

#include "Utils\Timer.h"
#include <crtdbg.h>

#include "Rendering\Renderer.h"
#include "Rendering\Model.h"

#include "Utils\Error.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ResourceManager.h"
#include "GLAbstractions\Texture.h"
#include "Graphics\Sprite.h"

#include "Rendering\ModelManager.h"
#include "Utils\ModelCreator.h"

#include "Game/Terrain\Terrain.h"

#include <random>

// ---------------------------------------- MAIN ----------------------------------------

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::srand(std::time(nullptr));

	Error::init();
	Display display("Project L", 600, 600);

	glEnable(GL_TEXTURE_2D);
	ResourceManager::initResources("DefaultImage.png");
	ResourceManager::addTexture("GrassTexture", "Textures/Terrain/Grass.png");
	ResourceManager::addTexture("DirtTexture", "Textures/Terrain/Dirt.png");

	ModelCreator creator;
	
	ModelManager::add("RectangleModel", ModelCreator::createRectangleModel(1.0f, 1.0f));
	
	Shader terrainShader("Shaders/Terrain/terrain.fs", "Shaders/Terrain/terrain.vs");
	Sprite sprite(ResourceManager::getTexture("GrassTexture"), Vec2(0.0f, 0.0f));
	sprite.setScale({0.25f, 0.25f});

	Sprite spriteDirt(ResourceManager::getTexture("DirtTexture"), Vec2(0.0f, -0.24f));
	spriteDirt.setScale({ 0.25f, 0.25f });

	Sprite spriteDirt2(ResourceManager::getTexture("DirtTexture"), Vec2(0.0f, -0.49f));
	spriteDirt2.setScale({ 0.25f, 0.25f });

	Terrain terrain;
	
	Renderer renderer;

	float dt = 0.0f;
	Timer dtTimer;

	float fps = 0.0f;
	Timer fpsTimer;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (display.isOpen())
	{
		display.processEvents();

		display.getWindowPtr()->setActive();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dtTimer.restart();

		Shader* shader = renderer.getDefaultShader();

		renderer.bindShader();
		/*shader->setUniform3f("tint", 1.0f, 1.0f, 1.0f);
		shader->setTexture2D("myTexture", 0, ResourceManager::getDefaultTexture()->getID());
		Model* mc = ModelManager::get("RectangleModelInstanced");
		if(mc->isInstanced)
			renderer.drawInstanced(mc->va, mc->ib, mc->count);
		else
			renderer.draw(mc->va, mc->ib);
		*/

		renderer.drawSprite(sprite, terrainShader);
		renderer.drawSprite(spriteDirt, terrainShader);
		renderer.drawSprite(spriteDirt2, terrainShader);

		terrain.draw(renderer);

		display.setTitleSufix(" dt: " + std::to_string(dt) + ", FPS: " + std::to_string(fps));

		fpsTimer.update();
		if (fpsTimer.getTime() > 1.0f)
		{
			fps = 1.0f / dt;
			fpsTimer.restart();
		}

		dtTimer.stop();
		dt = dtTimer.getTime();
		display.getWindowPtr()->display();
	}

	ModelManager::removeAll();
	ResourceManager::freeResources();

	return 0;
}