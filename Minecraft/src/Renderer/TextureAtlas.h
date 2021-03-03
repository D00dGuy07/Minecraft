#pragma once

#include "glm/glm.hpp"

struct TextureCoords
{
	glm::vec2 v0;
	glm::vec2 v1;
	glm::vec2 v2;
	glm::vec2 v3;
};

class TextureAtlas
{
private:
	static float m_SizeX;
	static float m_SizeY;
public:
	static void SetAtlasSize(float sizeX, float sizeY);
	static TextureCoords GetCoords(float locX, float locY);
};