#pragma once

#include <string>
#include <vector>

#include "Renderer/ImageBuffer.h"

class Texture : public ImageBuffer
{
public:
	Texture(const std::string& path, int32_t magFilter);
	Texture(int32_t width, int32_t height, InternalImageFormat format);
	virtual ~Texture() override;

	void Bind(uint32_t slot = 0) const;
	void BindImage(uint32_t slot = 0);

	virtual void Reallocate(int32_t width, int32_t height) override;
	
private:
	uint8_t* m_LocalBuffer;
	int32_t m_BPP;
	InternalImageFormat m_Format;

	static std::vector<uint32_t> m_BoundTextureUnits;
	static uint32_t m_MaxTextureUnits;

	static std::vector<uint32_t> m_BoundImageUnits;
	static uint32_t m_MaxImageUnits;

	static void ReserveBindings();
	static void FreeBinding(uint32_t rendererId);
};