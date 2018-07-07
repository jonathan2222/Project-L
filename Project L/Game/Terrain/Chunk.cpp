#include "Chunk.h"

#include "../../Utils/ModelCreator.h"

#include "TerrainConfig.h"

#include "../../Rendering/Renderer.h"

Chunk::Chunk()
{
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
		createModel(i);
}

Chunk::~Chunk()
{
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
		delete this->models[i];
}

void Chunk::createModel(unsigned int layer)
{
	this->models[layer] = ModelCreator::createRectangleModel(1.0f, 1.0f, TILE_IMG_SIZE);
	this->models[layer]->isInstanced = true;
	this->models[layer]->count = CHUNK_SIZE * CHUNK_SIZE;

	// Create buffer for translation
	/*this->models[layer]->vbInstanced.make(NULL, sizeof(float) * 2 * CHUNK_SIZE*CHUNK_SIZE, GL_STREAM_DRAW);
	VertexBufferLayout instancedLayout;
	this->models[layer]->va.addBuffer(this->models[layer]->vbInstanced, instancedLayout, true);
	*/

	// Create buffer for min uv, use same instanced layout
	this->models[layer]->vbInstanced2.make(NULL, sizeof(Tile) * CHUNK_SIZE*CHUNK_SIZE, GL_STREAM_DRAW);
	VertexBufferLayout instancedLayoutMinUv;
	instancedLayoutMinUv.push<float>(2); // Translation
	instancedLayoutMinUv.push<float>(2); // Min uv
	instancedLayoutMinUv.push<float>(2); // Min uv2
	instancedLayoutMinUv.push<float>(2); // Min uv left
	instancedLayoutMinUv.push<float>(2); // Min uv right
	instancedLayoutMinUv.push<float>(2); // Min uv up
	instancedLayoutMinUv.push<float>(2); // Min uv down
	instancedLayoutMinUv.push<float>(2); // Min uv mask
	instancedLayoutMinUv.push<float>(4); // Mask side (Which corner) (tl, tr, bl, br)
	instancedLayoutMinUv.push<unsigned int>(1); // Random bits.
	instancedLayoutMinUv.push<unsigned int>(1); // Corners.
	this->models[layer]->va.addBuffer(this->models[layer]->vbInstanced2, instancedLayoutMinUv, true);
}

void Chunk::draw(const Renderer & renderer, unsigned int layer, bool useWireframe) const
{
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
	{
		if (useWireframe)
			renderer.drawInstanced(GL_LINE_LOOP, this->models[i]->va, this->models[i]->ib, this->models[i]->count);
		else
			renderer.drawInstanced(this->models[i]->va, this->models[i]->ib, this->models[i]->count);
	}
}

void Chunk::updateLayers()
{
	for (unsigned int i = 0; i < TILE_LAYERS; i++)
		updateLayer(i);
}

void Chunk::updateLayer(unsigned int layer)
{
	this->models[layer]->vbInstanced2.updateData(&this->tiles[layer][0][0].pos, sizeof(Tile)*this->models[layer]->count);
}
