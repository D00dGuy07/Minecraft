#pragma once

#include "glm/glm.hpp"

#include <cmath>

namespace UsefulMath
{
	glm::ivec3 getChunkFromPosition(glm::vec3 position)
	{
		int chunkX = (int)floor(floor(position.x + 0.5f) / 32.0f);
		int chunkY = (int)floor(floor(position.y + 0.5f) / 32.0f);
		int chunkZ = (int)floor(floor(position.z + 0.5f) / 32.0f);

		return glm::ivec3(chunkX, chunkY, chunkZ);
	}
}