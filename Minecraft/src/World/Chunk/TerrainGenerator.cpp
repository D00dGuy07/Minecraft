#include "TerrainGenerator.h"

#include "FastNoise/FastNoise.h"

#include <math.h>

#include <iostream>

TerrainGenerator::TerrainGenerator()
	: m_HeightMap(65536) {}

void TerrainGenerator::PreWorldGen()
{
	FastNoise::SmartNode<FastNoise::Simplex> fnSimplex = FastNoise::New<FastNoise::Simplex>();
	FastNoise::SmartNode<FastNoise::FractalFBm> fnFractal = FastNoise::New<FastNoise::FractalFBm>();

	fnFractal->SetSource(fnSimplex);
	fnFractal->SetOctaveCount(5);

	fnFractal->GenUniformGrid2D(m_HeightMap.data(), 0, 0, 256, 256, 0.005f, 69420);
}

uint16_t TerrainGenerator::GetBlock(int x, int y, int z)
{
	float rawNoise = m_HeightMap[x + (z << 8)];
	float positiveNoise = (rawNoise + 1) / 2;

	int sectionHeight = floor(positiveNoise * 60) + 1;

	if ((y <= sectionHeight) && (y > sectionHeight - 3))
		return 4;
	else if (y < sectionHeight - 2)
		return 3;

	return 0;
}
