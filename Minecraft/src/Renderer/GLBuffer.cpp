#include "Renderer/GLBuffer.h"

// None of these are in a renderer submit because data needs to be copied to the gpu asap
GLBuffer::GLBuffer()
{
	glGenBuffers(1, &m_RendererID);
}

void GLBuffer::SetContents(const void* data, size_t size, Target target, Usage usage)
{
	m_Target = target;
	m_Size = size;
	glBindBuffer(static_cast<uint32_t>(target), m_RendererID);
	glBufferData(static_cast<uint32_t>(target), size, data, static_cast<uint32_t>(usage));
}