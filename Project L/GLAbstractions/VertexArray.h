#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout, bool instanced = false);

	void bind() const;
	void unbind() const;

private:
	unsigned int id;
	unsigned int offset;
	unsigned int nextLocation;
};

#endif