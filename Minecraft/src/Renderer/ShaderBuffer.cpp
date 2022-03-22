#include "Renderer/ShaderBuffer.h"

#include "Renderer/Renderer.h"

#include <iostream>

uint32_t ShaderBuffer::m_BoundRendererID = 0xFFFFFFFF;

std::vector<uint32_t> ShaderBuffer::m_BoundIndexedTargets = std::vector<uint32_t>();
uint32_t ShaderBuffer::m_MaxIndexedTargets = 0U;

ShaderBuffer::ShaderBuffer(const void* data, size_t size)
	: GLBuffer()
{
	SetContents(data, size, Target::ShaderStorage, Usage::StreamDraw);

	if (m_MaxIndexedTargets == 0)
		ReserveBindings();
}

ShaderBuffer::~ShaderBuffer()
{
	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		if (m_BoundRendererID == id)
			m_BoundRendererID = 0xFFFFFFFF;
		FreeBinding(id);
		glDeleteBuffers(1, &id);
	});
}

void ShaderBuffer::Bind() const
{
	Renderer::Submit([=]() {
		if (m_Target == GLBuffer::Target::ShaderStorage && m_BoundRendererID == m_RendererID)
			return;
		m_BoundRendererID = m_RendererID;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	});
}

void ShaderBuffer::BindIndexed(uint32_t index) const
{
	if (m_MaxIndexedTargets == 0)
		ReserveBindings();

	if (index > m_MaxIndexedTargets - 1)
	{
		std::cout << "Tried to bind a buffer to buffer index " << index << " which is not a valid indexed buffer index!";
		return;
	}

	Bind();
	Renderer::Submit([=]() {
		if (m_Target == GLBuffer::Target::ShaderStorage && m_BoundIndexedTargets[index] == m_RendererID)
			return;
		m_BoundIndexedTargets[index] = m_RendererID;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_RendererID);
	});
}



void ShaderBuffer::ReserveBindings()
{
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, reinterpret_cast<int32_t*>(&m_MaxIndexedTargets));
	m_BoundIndexedTargets.assign(m_MaxIndexedTargets, 0xFFFFFFFF);
}

void ShaderBuffer::FreeBinding(uint32_t rendererId)
{
	for (uint32_t i = 0; i < m_MaxIndexedTargets; i++)
		if (m_BoundIndexedTargets[i] == rendererId)
			m_BoundIndexedTargets[i] = 0xFFFFFFFF;
}