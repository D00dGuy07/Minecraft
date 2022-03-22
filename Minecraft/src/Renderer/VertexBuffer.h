#pragma once

#include "Renderer/GLBuffer.h"

class VertexBuffer : public GLBuffer
{
public:
	VertexBuffer(const void* data, size_t size);
	virtual ~VertexBuffer();

	virtual void Bind() const override;
private:
	static uint32_t m_BoundRendererID;
};