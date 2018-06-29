#ifndef MODEL_CREATOR_H
#define MODEL_CREATOR_H

#include "../Maths/Vectors/Vec2.h"
#include "../Rendering/Model.h"

class ModelCreator
{
public:
	static Model * createRectangleModel(float width, float height, const Vec2 & origin = Vec2(0.0f, 0.0f))
	{
		Model* model = new Model();

		const float hw = width / 2.0f;
		const float hh = height / 2.0f;
		Vec2 tl(origin + Vec2(-hw, hh));
		Vec2 tr(origin + Vec2(hw, hh));
		Vec2 bl(origin + Vec2(-hw, -hh));
		Vec2 br(origin + Vec2(hw, -hh));
		
		Vertex vertices[4] = {
			{ tl, Vec2(0.0f, 0.0f) },
			{ tr, Vec2(1.0f, 0.0f) },
			{ br, Vec2(1.0f, 1.0f) },
			{ bl, Vec2(0.0f, 1.0f) }
		};

		model->vb.make(vertices, sizeof(Vertex) * 4);
		VertexBufferLayout layout;
		layout.push<float>(2); // Pos
		layout.push<float>(2); // Uvs
		model->va.addBuffer(model->vb, layout);

		unsigned int indices[6] = { 0, 2, 1, 0, 3, 2 };
		model->ib.make(indices, 6);

		model->isInstanced = false;
		return model;
	}
};

#endif
