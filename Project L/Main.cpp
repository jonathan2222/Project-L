#include <iostream>

#include "GUI\Display.h"

#include "Input\Input.h"

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

#include "GUI\Font.h"

#include "Rendering\ModelManager.h"
#include "Utils\ModelCreator.h"

#include "../../GUI/TextRenderer.h"
#include "Game/Terrain/Terrain.h"
#include "Game/Terrain/TerrainConfig.h"
#include "Game/Player/Player.h"

#include <random>

#include "Box2D\Box2D.h"
#include "Graphics\Sprite.h"
#include "Game\Physics\TerrainCollider.h"

// ---------------------------------------- MAIN ----------------------------------------

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::srand((unsigned int)time(nullptr));

	Error::init();
	Display display("Project L");

	Input::init(display.getWindowPtr());

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // For FreeType
	ResourceManager::initResources("DefaultImage.png");
	
	ModelManager::add("RectangleModel", ModelCreator::createRectangleModel(1.0f, 1.0f));

	TextRenderer textRenderer;
	Font font, fontSmall;
	Text infoText, typeText, maskText, selectText;
	font.load("./Resources/Fonts/kongtext.ttf", 12, &display);
	fontSmall.load("./Resources/Fonts/kongtext.ttf", 8, &display);
	textRenderer.setFont(&font);
	infoText.setText("Info", &fontSmall);
	infoText.setColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	maskText.setText("Mask", &fontSmall);
	maskText.setColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	typeText.setText("Type", &fontSmall);
	typeText.setColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	selectText.setText("Selected: ", &fontSmall);
	selectText.setColor({ 0.2f, 0.2f, 0.2f, 1.0f });

	float dt = 0.0f;
	Timer dtTimer;

	float fps = 0.0f;
	Timer fpsTimer;

	float physicsTime = 0.0f;
	Timer physicsTimer;

	b2Vec2 gravity(0.0f, -9.82f);
	b2World world(gravity);
	
	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	ResourceManager::addTexture("Box 16x16", "Textures/Box.png");
	Sprite spriteBox(ResourceManager::getTexture("Box 16x16"), Vec2(0.0f, 0.0f));
	spriteBox.setScale({ 2.0f, 2.0f });
	Shader spriteShader("Shaders/sprite.fs", "Shaders/sprite.vs");

	Player player(&world);
	Terrain terrain;
	Vec4 tileIndexInFocus;

	Renderer renderer;

	TerrainCollider terrainCollider(&world);
	terrainCollider.setTerrain(&terrain);
	terrainCollider.createBodies(player.getCamera().getPosition(), MIDDLE_TILE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	while (display.isOpen())
	{
		if (display.sizeUpdated)
		{
			std::cout << "Resized!" << std::endl;
			display.sizeUpdated = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dtTimer.restart();

		Shader* shader = renderer.getDefaultShader();

		renderer.bindShader();

		player.update(dt, &display, &terrain);
		terrainCollider.updateBodies(player.getCamera().getPosition(), MIDDLE_TILE);
		terrain.draw(player.getCamera(), renderer, &display);

		physicsTimer.update();
		if (physicsTimer.getTime() >= timeStep)
		{
			// Instruct the world to perform a single step of simulation.
			// It is generally best to keep the time step and iterations fixed.
			world.Step(timeStep, velocityIterations, positionIterations);

			physicsTimer.restart();
		}
		terrainCollider.drawBodies(renderer, spriteShader, player.getCamera().getMatrix());
		renderer.drawSprite(player.getSprite(), spriteShader, player.getCamera().getMatrix());

		
		selectText.setText("Selected: " + TileConfig::getStrFromType(player.getSelectedTile()));
		float scale = 1.0f;
		float height = infoText.getHeight() * 2.5f * display.getPixelYScale()*scale;
		textRenderer.renderText(infoText, -1.0f, 1.0f - height, scale, &display);
		height += typeText.getHeight() * 2.5f * display.getPixelYScale()*scale;
		textRenderer.renderText(typeText, -1.0f, 1.0f - height, scale, &display);
		height += maskText.getHeight() * 2.5f * display.getPixelYScale()*scale;
		textRenderer.renderText(maskText, -1.0f, 1.0f - height, scale, &display);
		height += selectText.getHeight() * 2.5f * display.getPixelYScale()*scale;
		textRenderer.renderText(selectText, -1.0f, 1.0f - height, scale, &display);
		
		tileIndexInFocus = player.getTileIndexInFocus();
		if (tileIndexInFocus.x != -1.0f)
		{
			const unsigned int v = (unsigned int)tileIndexInFocus.x;
			const unsigned int h = (unsigned int)tileIndexInFocus.y;
			const unsigned int x = (unsigned int)tileIndexInFocus.z;
			const unsigned int y = (unsigned int)tileIndexInFocus.w;
			Tile* tile = terrain.getTile(v, h, x, y, MIDDLE_TILE);
			infoText.setText("Tile pos: " + std::to_string(tile->pos.x) + ", " + std::to_string(tile->pos.y) + " [" + std::to_string(v) + ", " + std::to_string(h) + "][" + std::to_string(x) + ", " + std::to_string(y) + "]");
			typeText.setText("Type: " + TileConfig::getStrFromMinUv(tile->minUv));
			maskText.setText("MASK: " + TileConfig::getStrFromMinUvMask(tile->minUvMask));
		}
		
		//this->font.setSize(24);
		textRenderer.renderText("Another test!", -1.0, -1.0, 1.0f, &display);

		display.setTitleSufix(" dt: " + std::to_string(dt) + ", FPS: " + std::to_string(fps));

		fpsTimer.update();
		if (fpsTimer.getTime() > 1.0f)
		{
			fps = 1.0f / dt;
			fpsTimer.restart();
		}

		dtTimer.stop();
		dt = dtTimer.getTime();
		display.endFrame();
	}

	ModelManager::removeAll();
	ResourceManager::freeResources();
	Font::FREE_SHADER();

	return 0;
}