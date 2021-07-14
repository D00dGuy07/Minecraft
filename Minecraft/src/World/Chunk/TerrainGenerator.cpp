#include "TerrainGenerator.h"

#include "FastNoise/FastNoise.h"

#include <math.h>

#include <iostream>

TerrainGenerator::TerrainGenerator()
	: m_HeightMap(1024) {}

uint16_t TerrainGenerator::GetBlock(int localX, int localY, int localZ, int worldX, int worldY, int worldZ)
{
	int sectionHeight = getHeightAtBlock(localX, localZ);

	if (worldY == sectionHeight)
	{
		return 1;
	}
	else if (worldY < sectionHeight)
		return 3;

	return 0;
}

void TerrainGenerator::PreChunkGen(int x, int y, int z)
{
	loadChunkHeightmap(x, y, z);
}

void TerrainGenerator::loadChunkHeightmap(int x, int y, int z)
{
	FastNoise::SmartNode<FastNoise::Simplex> fnSimplex = FastNoise::New<FastNoise::Simplex>();
	FastNoise::SmartNode<FastNoise::FractalFBm> fnFractal = FastNoise::New<FastNoise::FractalFBm>();

	fnFractal->SetSource(fnSimplex);
	fnFractal->SetOctaveCount(4);

	int32_t startX = x * 32;
	int32_t startZ = z * 32;

	fnFractal->GenUniformGrid2D(m_HeightMap.data(), startX, startZ, 32, 32, 0.003f, 69420);
}

int TerrainGenerator::getHeightAtBlock(int x, int z)
{
	float rawNoise = m_HeightMap[x + (z << 5)];
	float positiveNoise = (rawNoise + 1) / 2;

	return floor(positiveNoise * 250) + 1;
}
