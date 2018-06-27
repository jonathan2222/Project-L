#include "IndexBuffer.h"

#include "GL\glew.h"

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count)
{
	make(data, count);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &this->id);
}

void IndexBuffer::make(const unsigned int * data, unsigned int count)
{
	this->count = count;
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
