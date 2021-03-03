#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();

	void Initialize(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
	bool m_Initialized;
};