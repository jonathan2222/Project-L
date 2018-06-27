#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <string>

class UniformBuffer
{
public:
	UniformBuffer(const std::string& name, unsigned int shader, unsigned int bindingPoint);
	~UniformBuffer();

	void setData(const void* data, unsigned int size);
	void setSubData(const void* data, unsigned int size, unsigned int offset); // setData needs to have been used before using this!

private:
	unsigned int id;
	unsigned int index;
	unsigned int bindingPoint;
};

#endif
