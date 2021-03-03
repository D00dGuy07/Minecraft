#include "TextureAtlas.h"

float TextureAtlas::m_SizeX = 0;
float TextureAtlas::m_SizeY = 0;

void TextureAtlas::SetAtlasSize(float sizeX, float sizeY)
{
	m_SizeX = sizeX;
	m_SizeY = sizeY;
}
TextureCoords TextureAtlas::GetCoords(float locX, float locY)
{
	float unitX = 1.0f / m_SizeX;
	float unitY = 1.0f / m_SizeY;

	return {
		glm::vec2(locX * unitX, 1.0f - ((locY + 1) * unitY)),
		glm::vec2((locX + 1) * unitX, 1.0f - ((locY + 1) * unitY)),
		glm::vec2((locX + 1) * unitX, 1.0f - (locY * unitY)),
		glm::vec2(locX * unitX, 1.0f - (locY * unitY))
	};
}