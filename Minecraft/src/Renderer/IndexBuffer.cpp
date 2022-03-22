#include "IndexBuffer.h"

#include "Renderer.h"

uint32_t IndexBuffer::m_BoundRendererID = 0xFFFFFFFF;

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
	: GLBuffer()
{
	SetContents(data, static_cast<size_t>(count * sizeof(uint32_t)),
		Target::ElementArray, Usage::StaticDraw);
}

IndexBuffer::~IndexBuffer()
{
	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		if (m_BoundRendererID == id)
			m_BoundRendererID = 0xFFFFFFFF;
		glDeleteBuffers(1, &id);
	});
}

void IndexBuffer::Bind() const
{
	Renderer::Submit([=]() {
		if (m_BoundRendererID == m_RendererID)
			return;
		m_BoundRendererID = m_RendererID;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	});
}
