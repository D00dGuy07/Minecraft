#include "Texture.h"

#include "glad/glad.h"
#include "stb_image/stb_image.h"

#include "Renderer/Renderer.h"

#include <iostream>

std::vector<uint32_t> Texture::m_BoundTextureUnits = std::vector<uint32_t>();
uint32_t Texture::m_MaxTextureUnits = 0U;
std::vector<uint32_t> Texture::m_BoundImageUnits = std::vector<uint32_t>();
uint32_t Texture::m_MaxImageUnits = 0U;

Texture::Texture(const std::string& path, int32_t magFilter)
	: ImageBuffer(NULL, 0, 0), m_LocalBuffer(nullptr), m_BPP(0), m_Format(InternalImageFormat::RGBA8)
{
	Renderer::Submit([&, path, magFilter]() {
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	});

	if (m_MaxTextureUnits == 0 || m_MaxImageUnits == 0)
		ReserveBindings();
}

Texture::Texture(int32_t width, int32_t height, InternalImageFormat format)
	: ImageBuffer(NULL, width, height), m_BPP(32), m_Format(format), m_LocalBuffer(NULL)
{
	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Format, sizedId, baseId);
	Renderer::Submit([&, sizedId, baseId]() {
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, sizedId, m_Width, m_Height, 0, baseId, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	});

	if (m_MaxTextureUnits == 0 || m_MaxImageUnits == 0)
		ReserveBindings();
}

Texture::~Texture()
{
	uint32_t id = m_RendererID;
	Renderer::Submit([=]() {
		FreeBinding(id);
		glDeleteTextures(1, &id);
	});
}

void Texture::Bind(uint32_t slot) const
{
	if (slot > m_MaxTextureUnits - 1)
	{
		std::cout << "Tried to bind a texture to texture slot " << slot << " which is not a valid texture slot!";
		return;
	}

	Renderer::Submit([=]() {
		if (m_BoundTextureUnits[slot] == m_RendererID)
			return;
		m_BoundTextureUnits[slot] = m_RendererID;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	});
}

void Texture::BindImage(uint32_t slot)
{
	if (slot > m_MaxImageUnits - 1)
	{
		std::cout << "Tried to bind a texture to image slot " << slot << " which is not a valid image slot!";
		return;
	}

	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Format, sizedId, baseId);
	Renderer::Submit([=]() {
		if (m_BoundImageUnits[slot] == m_RendererID)
			return;
		m_BoundImageUnits[slot] = m_RendererID;
		glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, sizedId);
	});
}

void Texture::Reallocate(int32_t width, int32_t height)
{
	m_Width = width;
	m_Height = height;

	int32_t sizedId = 0, baseId = 0;
	ImageBuffer::GetFormatData(m_Format, sizedId, baseId);

	Bind();
	Renderer::Submit([=]() {
		glTexImage2D(GL_TEXTURE_2D, 0, sizedId, m_Width, m_Height, 0, baseId, GL_UNSIGNED_BYTE, NULL);
	});
}

void Texture::ReserveBindings()
{
	// Get max bindings count
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, reinterpret_cast<int32_t*>(&m_MaxTextureUnits));
	glGetIntegerv(GL_MAX_IMAGE_UNITS, reinterpret_cast<int32_t*>(&m_MaxImageUnits));

	// Set initial values
	m_BoundTextureUnits.assign(m_MaxTextureUnits, 0xFFFFFFFF);
	m_BoundImageUnits.assign(m_MaxImageUnits, 0xFFFFFFFF);
}

void Texture::FreeBinding(uint32_t rendererId)
{
	for (uint32_t i = 0; i < m_MaxTextureUnits; i++)
		if (m_BoundTextureUnits[i] == rendererId)
			m_BoundTextureUnits[i] = 0xFFFFFFFF;
	for (uint32_t i = 0; i < m_MaxImageUnits; i++)
		if (m_BoundImageUnits[i] == rendererId)
			m_BoundImageUnits[i] = 0xFFFFFFFF;
}
