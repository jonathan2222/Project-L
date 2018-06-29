#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <vector>

#include "GL\glew.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : stride(0) {}

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		this->elements.push_back({ GL_FLOAT, count, GL_FALSE });
		this->stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		this->elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		this->stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		this->elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		this->stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	void pushMat3()
	{
		push<float>(3);
		push<float>(3);
		push<float>(3);
	}

	void pushMat4()
	{
		push<float>(4);
		push<float>(4);
		push<float>(4);
		push<float>(4);
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return this->elements; }
	inline unsigned int getStride() const { return this->stride; }

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;
};

#endif
