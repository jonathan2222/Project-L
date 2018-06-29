#ifndef MODEL_H
#define MODEL_H

#include "../GLAbstractions/VertexArray.h"
#include "../GLAbstractions/IndexBuffer.h"

#include "../Maths/Vectors/Vec2.h"

struct Vertex
{
	Vec2 pos;
	Vec2 uv;
};

struct Model
{
	VertexArray va;
	VertexBuffer vb;
	VertexBuffer vbInstanced;
	IndexBuffer ib;

	bool isInstanced = false;
	unsigned int count = 0; // Number of instances.
};

#endif
