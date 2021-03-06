#include "TextureAtlas.h"

float TextureAtlas::m_SizeX = 0;
float TextureAtlas::m_SizeY = 0;

void TextureAtlas::SetAtlasSize(float sizeX, float sizeY)
{
	m_SizeX = sizeX;
	m_SizeY = sizeY;
}
TextureCoords TextureAtlas::GetCoords(float x, float y)
{
	float texWidth = m_SizeX * 16.0f;
	float texHeight = m_SizeY * 16.0f;

	return {
		glm::vec2(((x * 16.0f))        , (texHeight - ((y * 16.0f) + 16.0f))),
		glm::vec2(((x * 16.0f) + 16.0f), (texHeight - ((y * 16.0f) + 16.0f))),
		glm::vec2(((x * 16.0f) + 16.0f), (texHeight - ((y * 16.0f) - 0.0f))),
		glm::vec2(((x * 16.0f))        , (texHeight - ((y * 16.0f) - 0.0f)))
	};


	//float unitX = 1.0f / m_SizeX;
	//float unitY = 1.0f / m_SizeY;

	//float halfTexelX = 1.0f / (16.0f * m_SizeX * 2.0f);
	//float halfTexelY = 1.0f / (16.0f * m_SizeY * 2.0f);

	//return {
	//	glm::vec2((x * unitX)       , (1.0f - ((y + 1) * unitY))),
	//	glm::vec2(((x + 1) * unitX) , (1.0f - ((y + 1) * unitY))),
	//	glm::vec2(((x + 1) * unitX) , (1.0f - (y * unitY))),
	//	glm::vec2((x * unitX)       , (1.0f - (y * unitY)))
	//};
}