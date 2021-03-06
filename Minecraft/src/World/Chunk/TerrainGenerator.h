#pragma once

#include <cstdint>
#include <vector>

class TerrainGenerator
{
public:
	TerrainGenerator();

	void PreWorldGen();

	uint16_t GetBlock(int x, int y, int z);

private:
	std::vector<float> m_HeightMap;
};