#ifndef RENDERER_H
#define RENDERER_H

#include "..\GLAbstractions\VertexArray.h"
#include "..\GLAbstractions\IndexBuffer.h"
#include "..\Shaders\Shader.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void bindShader(const Shader* shader = nullptr) const;
	void draw(const VertexArray& va, const IndexBuffer& ib) const;

	Shader* getDefaultShader() const;

private:
	Shader * defaultShader;
};

#endif
