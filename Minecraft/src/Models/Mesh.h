#pragma once

#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
	~Mesh();

	void SetVertices(float vertices[], unsigned int size);
	void SetIndices(unsigned int indices[], unsigned int length);

	bool BuildVertexArray();
	unsigned int Bind();
	void Unbind();

	bool isVAOBuilt() const { return m_VertexArrayBuilt; };
	
	VertexBufferLayout BufferLayout = VertexBufferLayout();
private:
	VertexArray* m_VertexArray;
	bool m_VertexArrayBuilt;
	VertexBuffer* m_VertexBuffer;
	bool m_VerticesSet;
	IndexBuffer* m_IndexBuffer;
	bool m_IndicesSet;
};