#include <iostream>

#include "GUI\Display.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

#include "Utils\Timer.h"
#include <crtdbg.h>

#include "Rendering\Renderer.h"
#include "Rendering\Model.h"

#include "Utils\Error.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ResourceManager.h"
#include "GLAbstractions\Texture.h"

#include <random>

// ---------------------------------------- MAIN ----------------------------------------

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::srand(std::time(nullptr));

	Error::init();
	ResourceManager::initResources("DefaultImage.png");

	Display display("Project L", 600, 600);

	Model m;

	struct Vertex
	{
		Vec2 pos;
		Vec2 uv;
	};
	Vertex data[4] = {
		{ Vec2(-1.0f, 1.0f), Vec2(0.0f, 1.0f) },
		{ Vec2(1.0f, 1.0f), Vec2(1.0f, 1.0f) },
		{ Vec2(1.0f, -1.0f), Vec2(1.0f, 0.0f) },
		{ Vec2(-1.0f, -1.0f), Vec2(0.0f, 0.0f) }
	};
	VertexBuffer vb(data, sizeof(Vertex) * 4);
	VertexBufferLayout layout;
	layout.push<float>(2); // Pos
	layout.push<float>(2); // Uvs
	m.va.addBuffer(vb, layout);

	const unsigned int NUMBER_OF_INSTANCES = 100;
	Vec2 offsets[NUMBER_OF_INSTANCES];
	for (unsigned int i = 0; i < NUMBER_OF_INSTANCES; i++)
	{
		offsets[i].x = ((std::rand() % 100) / 100.0f) * 2.0f - 1.0f;
		offsets[i].y = ((std::rand() % 100) / 100.0f) * 2.0f - 1.0f;
	}
	VertexBuffer vbInstanceData(offsets, sizeof(Vec2)*NUMBER_OF_INSTANCES);
	VertexBufferLayout instancedLayout;
	instancedLayout.push<float>(2); // Offset
	m.va.addBuffer(vbInstanceData, instancedLayout, true);

	unsigned int indices[] = { 0, 2, 1, 0, 3, 2 };
	m.ib.make(indices, 6);

	Texture texture(ResourceManager::getDefaultImage());

	Renderer renderer;

	float dt = 0.0f;
	Timer dtTimer;

	float fps = 0.0f;
	Timer fpsTimer;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (display.isOpen())
	{
		display.processEvents();

		display.getWindowPtr()->setActive();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dtTimer.restart();

		Shader* shader = renderer.getDefaultShader();

		renderer.bindShader();
		shader->setUniform2f("scale", 0.025f, 0.025f);
		shader->setUniform3f("tint", 1.0f, 1.0f, 1.0f);
		shader->setTexture2D("myTexture", 0, texture.getID());
		renderer.drawInstanced(m.va, m.ib, NUMBER_OF_INSTANCES);

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

	ResourceManager::freeResources();

	return 0;
}