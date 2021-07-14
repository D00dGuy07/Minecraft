#pragma once

#include <cstdint>
#include <vector>

class TerrainGenerator
{
public:
	TerrainGenerator();

	void PreChunkGen(int x, int y, int z);

	uint16_t GetBlock(int localX, int localY, int localZ, int worldX, int worldY, int worldZ);

private:
	std::vector<float> m_HeightMap;

	void loadChunkHeightmap(int x, int y, int z);

	int getHeightAtBlock(int x, int z);
};