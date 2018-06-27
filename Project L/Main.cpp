#include <iostream>

#include "GUI\Display.h"

#include "Maths\Vectors\Vec2.h"
#include "Maths\Vectors\Vec3.h"

#include "Utils\Timer.h"
#include <crtdbg.h>

#include "Rendering\Renderer.h"
#include "Rendering\Model.h"

// ---------------------------------------- MAIN ----------------------------------------

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Display display("Project T", 600, 600);

	Model m;

	struct Vertex
	{
		Vec2 pos;
		Vec3 color;
	};
	Vertex data[4] = {
		{ Vec2(-1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f) },
		{ Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f) },
		{ Vec2(1.0f, -1.0f), Vec3(0.0f, 0.0f, 1.0f) },
		{ Vec2(-1.0f, -1.0f), Vec3(1.0f, 0.0f, 1.0f) }
	};
	VertexBuffer vb(data, sizeof(Vertex) * 4);
	VertexBufferLayout layout;
	layout.push<float>(2); // Pos
	layout.push<float>(3); // Color
	m.va.addBuffer(vb, layout);
	unsigned int indices[] = { 0, 2, 1, 0, 3, 2 };
	m.ib.make(indices, 6);

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
		shader->setUniform2f("scale", 0.5f, 0.5f);
		shader->setUniform2f("offset", 0.0f, 0.0f);
		shader->setUniform3f("tint", 1.0f, 1.0f, 1.0f);
		renderer.draw(m.va, m.ib);

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

	return 0;
}