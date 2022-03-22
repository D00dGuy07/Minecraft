#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

#include <iostream>

uint32_t VertexArray::m_BoundRendererID = 0xFFFFFFFF;

VertexArray::VertexArray()
	: m_RendererID(NULL)
{
	Renderer::Submit([&]() {
		glGenVertexArrays(1, &m_RendererID);
	});
	
}

VertexArray::~VertexArray()
{
	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		if (m_BoundRendererID == id)
			m_BoundRendererID = 0xFFFFFFFF;
		glDeleteVertexArrays(1, &id);
	});
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	uint64_t offset = 0;
	for (uint32_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		uint32_t stride = layout.GetStride();
		Renderer::Submit([=]() {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type,
				element.normalized, stride, reinterpret_cast<const void*>(offset));
		});
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	Renderer::Submit([=]() {
		if (m_BoundRendererID == m_RendererID)
			return;
		m_BoundRendererID = m_RendererID;
		glBindVertexArray(m_RendererID);
	});
}