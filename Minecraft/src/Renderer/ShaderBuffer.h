#pragma once

#include "Renderer/GLBuffer.h"

#include <vector>

class ShaderBuffer : public GLBuffer
{
public:
	ShaderBuffer(const void* data, size_t size);
	virtual ~ShaderBuffer();

	virtual void Bind() const override;
	void BindIndexed(uint32_t index = 0) const;
private:
	static uint32_t m_BoundRendererID;

	static std::vector<uint32_t> m_BoundIndexedTargets;
	static uint32_t m_MaxIndexedTargets;

	static void ReserveBindings();
	void FreeBinding(uint32_t rendererId);
};