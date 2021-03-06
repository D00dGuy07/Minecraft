#include "ChunkManager.h"

#include "ChunkMeshGenerator.h"

#include <cmath>
#include <iostream>

#define LOADED_CHUNK_SCALE 512

ChunkManager::ChunkManager()
	: m_CurrentChunk(0, 0, 0), m_NeedsGenerating(true), m_TerrainGenerator()
{
	m_Chunks.reserve(LOADED_CHUNK_SCALE);
	m_Meshes.reserve(LOADED_CHUNK_SCALE);

	for (int i = 0; i < LOADED_CHUNK_SCALE; i++)
	{
		m_Chunks.push_back(new Chunk());
		m_Meshes.push_back(new Mesh());
	}
}

ChunkManager::~ChunkManager()
{
	for (int i = 0; i < LOADED_CHUNK_SCALE; i++)
	{
		delete m_Chunks[i];
		delete m_Meshes[i];
	}
}

void ChunkManager::GenerateTerrain()
{
	m_TerrainGenerator.PreWorldGen();

	for (int chunkY = 0; chunkY < 2; chunkY++)
	{
		for (int chunkZ = 0; chunkZ < 8; chunkZ++)
		{
			for (int chunkX = 0; chunkX < 8; chunkX++)
			{
				for (int blockY = 0; blockY < 32; blockY++)
				{
					for (int blockZ = 0; blockZ < 32; blockZ++)
					{
						for (int blockX = 0; blockX < 32; blockX++)
						{
							int x = blockX + (chunkX * 32);
							int y = blockY + (chunkY * 32);
							int z = blockZ + (chunkZ * 32);

							getChunk(chunkX, chunkY, chunkZ)->setBlock(m_TerrainGenerator.GetBlock(x, y, z), blockX, blockY, blockZ);
						}
					}
				}
			}
		}
	}
}

void ChunkManager::GenerateMeshes()
{
	int x = m_CurrentChunk.x;
	int y = m_CurrentChunk.y;
	int z = m_CurrentChunk.z;

	SurroundingChunks neighboringChunks = {
		(z - 1 >= 0) ? getChunk(x, y, z - 1) : nullptr,
		(z + 1 <  8) ? getChunk(x, y, z + 1) : nullptr,
		(x - 1 >= 0) ? getChunk(x - 1, y, z) : nullptr,
		(x + 1 <  8) ? getChunk(x + 1, y, z) : nullptr,
		(y + 1 <  8) ? getChunk(x, y + 1, z) : nullptr,
		(y - 1 >= 0) ? getChunk(x, y - 1, z) : nullptr
	};

	ChunkMeshGenerator::generateChunkMesh(*getMesh(x, y, z), *getChunk(x, y, z), neighboringChunks, x, y, z);

	m_CurrentChunk.x = m_CurrentChunk.x + 1;

	if (m_CurrentChunk.x == 8)
	{
		m_CurrentChunk.x = 0;
		m_CurrentChunk.z = m_CurrentChunk.z + 1;
	}

	if (m_CurrentChunk.z == 8)
	{
		m_CurrentChunk.z = 0;
		m_CurrentChunk.y = m_CurrentChunk.y + 1;
	}

	if (m_CurrentChunk.y == 8)
	{
		m_NeedsGenerating = false;
	}
}
