#include "Renderer.h"

#include "..\Maths\Vectors\Vec2.h"
#include "GL\glew.h"

Renderer::Renderer()
{
	this->defaultShader = new Shader("Shaders/test.fs", "Shaders/test.vs");
}

Renderer::~Renderer()
{
	delete this->defaultShader;
}

void Renderer::bindShader(const Shader * shader) const
{
	if (shader != nullptr)
		shader->bind();
	else
		this->defaultShader->bind();
}

void Renderer::draw(const VertexArray & va, const IndexBuffer & ib) const
{
	va.bind();
	ib.bind();
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawInstanced(const VertexArray & va, const IndexBuffer & ib, GLsizei instanceCount) const
{
	va.bind();
	ib.bind();
	glDrawElementsInstanced(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0, instanceCount);
}

Shader * Renderer::getDefaultShader() const
{
	return this->defaultShader;
}
