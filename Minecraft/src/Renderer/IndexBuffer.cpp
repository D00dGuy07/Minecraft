#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count), m_Initialized(true)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer() : m_Initialized(false), m_Count(NULL), m_RendererID(NULL) {}

void IndexBuffer::Initialize(const unsigned int* data, unsigned int count)
{
    if (!m_Initialized)
    {
        m_Count = count;
        m_Initialized = true;

        ASSERT(sizeof(unsigned int) == sizeof(GLuint));

        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }
}

IndexBuffer::~IndexBuffer()
{
    if (m_Initialized)
    {
        glDeleteBuffers(1, &m_RendererID);
    }
}

void IndexBuffer::Bind() const
{
    if (m_Initialized)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
}

void IndexBuffer::Unbind() const
{
    if (m_Initialized)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
