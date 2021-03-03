#include "Mesh.h"

#include <iterator>

Mesh::~Mesh()
{
	delete m_VertexArray;
	delete m_VertexBuffer;
	delete m_IndexBuffer;
}

void Mesh::SetVertices(float vertices[], unsigned int size)
{
	if (!m_VerticesSet)
	{
		m_VertexBuffer = new VertexBuffer(vertices, size);
		m_VerticesSet = true;
	}
}

void Mesh::SetIndices(unsigned int indices[], unsigned int length) 
{
	if (!m_IndicesSet)
	{
		m_IndexBuffer = new IndexBuffer(indices, length);
		m_IndicesSet = true;
	}
}

bool Mesh::BuildVertexArray()
{
	if (m_VerticesSet && !m_VertexArrayBuilt)
	{
		m_VertexArray = new VertexArray();
		m_VertexArray->AddBuffer(*m_VertexBuffer, BufferLayout);
		m_VertexArrayBuilt = true;
		return true;
	}
	else
		return false;
}

unsigned int Mesh::Bind()
{
	if (m_VertexArrayBuilt && m_IndicesSet)
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		return m_IndexBuffer->GetCount();
	}
	return NULL;
}

void Mesh::Unbind()
{
	if (m_VertexArrayBuilt && m_IndicesSet)
	{
		m_VertexArray->Unbind();
		m_IndexBuffer->Unbind();
	}
}
