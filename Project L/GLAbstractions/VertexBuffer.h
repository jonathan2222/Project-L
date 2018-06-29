#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "GL\glew.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
	~VertexBuffer();

	void make(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
	void updateData(const void* data, unsigned int size);

	void bind() const;
	void unbind() const;

private:
	unsigned int id;
	bool hasData;
};

#endif
