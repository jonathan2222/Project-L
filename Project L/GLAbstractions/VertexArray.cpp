#include "VertexArray.h"

VertexArray::VertexArray() : offset(0), nextLocation(0)
{
	glGenVertexArrays(1, &this->id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &this->id);
}

void VertexArray::addBuffer(const VertexBuffer & vertexBuffer, const VertexBufferLayout & layout, bool instanced)
{
	bind();
	vertexBuffer.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray((int)this->nextLocation);
		if (element.type == GL_UNSIGNED_INT)
		{
			glVertexAttribIPointer((int)this->nextLocation, element.count, element.type, layout.getStride(), (const void*)offset);
		}
		else
		{
			glVertexAttribPointer((int)this->nextLocation, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		}
		offset += element.count*VertexBufferElement::getSizeOfType(element.type);
		if (instanced)
			glVertexAttribDivisor(this->nextLocation, 1);
		this->nextLocation++;
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(this->id);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
