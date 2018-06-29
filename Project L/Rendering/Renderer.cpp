#include "Renderer.h"

#include "..\Maths\Vectors\Vec2.h"
#include "GL\glew.h"
#include "../Graphics/Sprite.h"

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

void Renderer::drawSprite(const Sprite & sprite, Shader& shader) const
{
	shader.bind();
	shader.setUniformMatrix3fv("matrix", 1, false, &(sprite.getMatrix()[0][0]));
	shader.setUniform3fv("tint", 1, &sprite.getTint()[0]);
	shader.setTexture2D("tex", 0, sprite.getTexture()->getID());
	draw(sprite.getModel()->va, sprite.getModel()->ib);
}

Shader * Renderer::getDefaultShader() const
{
	return this->defaultShader;
}
