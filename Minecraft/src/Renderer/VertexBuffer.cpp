#include "VertexBuffer.h"

#include "Renderer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    : m_Initialized(true)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer() : m_Initialized(false), m_RendererID(NULL) {}

VertexBuffer::~VertexBuffer()
{
    if (m_Initialized)
        glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Initialize(const void* data, unsigned int size)
{
    if (!m_Initialized)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_Initialized = true;
    }
}

void VertexBuffer::Bind() const
{
    if (m_Initialized)
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    if (m_Initialized)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}
