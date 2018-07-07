#include "VertexBuffer.h"

#include "GL\glew.h"

VertexBuffer::VertexBuffer() : hasData(false)
{
}

VertexBuffer::VertexBuffer(const void * data, unsigned int size, GLenum usage)
{
	make(data, size, usage);
}

VertexBuffer::~VertexBuffer()
{
	if(this->hasData)
		glDeleteBuffers(1, &this->id);
}

void VertexBuffer::make(const void * data, unsigned int size, GLenum usage)
{
	this->hasData = true;
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBuffer::updateData(const void * data, unsigned int size, int offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, size, data);
}

void VertexBuffer::bind() const
{
	if(this->hasData)
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
