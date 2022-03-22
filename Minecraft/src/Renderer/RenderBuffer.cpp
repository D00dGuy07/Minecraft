#include "Renderer/RenderBuffer.h"

#include "Renderer/Renderer.h"

#include "glad/glad.h"

uint32_t RenderBuffer::m_BoundRendererID = 0xFFFFFFFF;

RenderBuffer::RenderBuffer(int32_t width, int32_t height, InternalImageFormat format)
	: ImageBuffer(NULL, width, height), m_Format(format)
{
	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Format, sizedId, baseId);
	Renderer::Submit([&, sizedId]() {
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);

		glRenderbufferStorage(GL_RENDERBUFFER, sizedId, m_Width, m_Height);
	});
}

RenderBuffer::~RenderBuffer()
{
	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		if (m_BoundRendererID == id)
			m_BoundRendererID = 0xFFFFFFFF;
		glDeleteRenderbuffers(1, &id);
	});
}

void RenderBuffer::Bind()
{
	Renderer::Submit([=]() {
		if (m_BoundRendererID == m_RendererID)
			return;
		m_BoundRendererID = m_RendererID;
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	});
}

void RenderBuffer::Reallocate(int32_t width, int32_t height)
{
	m_Width = width;
	m_Height = height;

	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Format, sizedId, baseId);

	Bind();
	Renderer::Submit([=]() {
		glRenderbufferStorage(GL_RENDERBUFFER, sizedId, m_Width, m_Height);
	});
}