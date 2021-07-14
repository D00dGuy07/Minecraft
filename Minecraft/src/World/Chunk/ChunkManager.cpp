#include "ChunkManager.h"

#include "ChunkMeshGenerator.h"

#include <stdexcept>
#include <cmath>
#include <iostream>

#define LOADED_CHUNK_SCALE 729

ChunkManager::ChunkManager(int x, int y, int z)
	: m_CenterChunk(x, y, z), m_TerrainGenerator(), m_Chunks()
{
	m_Chunks.reserve(LOADED_CHUNK_SCALE + 300);

	glm::ivec3 minBound = glm::ivec3(m_CenterChunk.x - 4, m_CenterChunk.y - 4, m_CenterChunk.z - 4);
	glm::ivec3 maxBound = glm::ivec3(m_CenterChunk.x + 4, m_CenterChunk.y + 4, m_CenterChunk.z + 4);

	for (int y = minBound.y; y <= maxBound.y; y++)
	{
		for (int z = minBound.z; z <= maxBound.z; z++)
		{
			for (int x = minBound.x; x <= maxBound.x; x++)
			{
				Chunk* chunk = new Chunk();
				Mesh* mesh = new Mesh();

				m_Chunks[Vec3Hash(x, y, z)] = { chunk, mesh };

				m_ChunksToGenerate.push_back({
					glm::ivec3(x, y, z),
					true,	// Generate Blocks
					true,	// Generate Mesh
					true,	// Generate Neighbor's Mesh
				});
			}
		}
	}
}

ChunkManager::~ChunkManager()
{
	glm::ivec3 minBound = glm::ivec3(m_CenterChunk.x - 4, m_CenterChunk.y - 4, m_CenterChunk.z - 4);
	glm::ivec3 maxBound = glm::ivec3(m_CenterChunk.x + 4, m_CenterChunk.y + 4, m_CenterChunk.z + 4);

	for (int y = minBound.y; y <= maxBound.y; y++)
	{
		for (int z = minBound.z; z <= maxBound.z; z++)
		{
			for (int x = minBound.x; x <= maxBound.x; x++)
			{
				UnloadChunk(x, y, z);
			}
		}
	}
}

void ChunkManager::LoadChunk(int x, int y, int z)
{
	if (isLoaded(x, y, z)) 
		return;

	Chunk* chunk = new Chunk();
	Mesh* mesh = new Mesh();

	m_Chunks[Vec3Hash(x, y, z)] = { chunk, mesh };

	m_ChunksToGenerate.push_back({
		glm::ivec3(x, y, z),
		true,	// Generate Blocks
		true,	// Generate Mesh
		true,	// Generate Neighbor's Mesh
	});
}

void ChunkManager::UnloadChunk(int x, int y, int z)
{
	if (!isLoaded(x, y, z))
		return;

	ChunkPair chunkData = m_Chunks[Vec3Hash(x, y, z)];

	delete chunkData.Chunk;
	delete chunkData.Mesh;

	m_Chunks.erase(Vec3Hash(x, y, z));
}

void ChunkManager::GenerateChunk()
{
	if (m_ChunksToGenerate.empty())
		return;

	ChunkGenerationPacket genPacket = *m_ChunksToGenerate.begin();

	int x = genPacket.Coords.x;
	int y = genPacket.Coords.y;
	int z = genPacket.Coords.z;

	Chunk* chunk = getChunk(x, y, z);

	if (chunk == nullptr)
	{
		m_ChunksToGenerate.erase(m_ChunksToGenerate.begin());
		return;
	}

	if (genPacket.GenerateBlocks)
		GenerateBlocks(chunk, x, y, z);

	SurroundingChunks neighbors;
	if (genPacket.BuildNeighbors)
		neighbors = getNeighboringChunks(x, y, z);

	bool builtMesh = false;
	if (genPacket.BuildMesh)
		builtMesh = GenerateMesh(x, y, z);

	if (builtMesh && genPacket.BuildNeighbors)
	{
		if (neighbors.Front != nullptr)
		{
			ChunkPair& pair = m_Chunks[Vec3Hash(x, y, z + 1)];
			delete pair.Mesh;
			pair.Mesh = new Mesh();
			GenerateMesh(x, y, z + 1);
		}
		if (neighbors.Back != nullptr)
		{
			ChunkPair& pair = m_Chunks[Vec3Hash(x, y, z - 1)];
			delete pair.Mesh;
			pair.Mesh = new Mesh();
			GenerateMesh(x, y, z - 1);
		}
		if (neighbors.Left != nullptr)
		{
			ChunkPair& pair = m_Chunks[Vec3Hash(x - 1, y, z)];
			delete pair.Mesh;
			pair.Mesh = new Mesh();
			GenerateMesh(x - 1, y, z);
		}
		if (neighbors.Right != nullptr)
		{
			ChunkPair& pair = m_Chunks[Vec3Hash(x + 1, y, z)];
			delete pair.Mesh;
			pair.Mesh = new Mesh();
			GenerateMesh(x + 1, y, z);
		}
		if (neighbors.Top != nullptr)
		{
			ChunkPair& pair = m_Chunks[Vec3Hash(x, y + 1, z)];
			delete pair.Mesh;
			pair.Mesh = new Mesh();
			GenerateMesh(x, y + 1, z);
		}
		if (neighbors.Bottom != nullptr)
		{
			ChunkPair& pair = m_Chunks[Vec3Hash(x, y - 1, z)];
			delete pair.Mesh;
			pair.Mesh = new Mesh();
			GenerateMesh(x, y - 1, z);
		}
	}

	m_ChunksToGenerate.erase(m_ChunksToGenerate.begin());
}

void ChunkManager::GenerateBlocks(Chunk* chunk, int x, int y, int z)
{
	m_TerrainGenerator.PreChunkGen(x, y, z);

	for (int localY = 0; localY < 32; localY++)
	{
		for (int localZ = 0; localZ < 32; localZ++)
		{
			for (int localX = 0; localX < 32; localX++)
			{
				int worldX = localX + (x * 32);
				int worldY = localY + (y * 32);
				int worldZ = localZ + (z * 32);

				uint16_t block = m_TerrainGenerator.GetBlock(localX, localY, localZ, worldX, worldY, worldZ);
				chunk->setBlock(block, localX, localY, localZ);
			}
		}
	}
}

bool ChunkManager::GenerateMesh(int x, int y, int z)
{
	//std::cout << "Generating Mesh...\n";

	SurroundingChunks neighboringChunks = getNeighboringChunks(x, y, z);

	bool result = ChunkMeshGenerator::generateChunkMesh(
		*getMesh(x, y, z), 
		*getChunk(x, y, z), 
		neighboringChunks, 
		x, y, z
	);

	return result;
}

void ChunkManager::SetMiddleChunk(glm::ivec3 chunk)
{
	// Calculate new min max for center
	glm::ivec3 oldMinBound = glm::ivec3(m_CenterChunk.x - 4, m_CenterChunk.y - 4, m_CenterChunk.z - 4);
	glm::ivec3 oldMaxBound = glm::ivec3(m_CenterChunk.x + 4, m_CenterChunk.y + 4, m_CenterChunk.z + 4);
	glm::ivec3 newMinBound = glm::ivec3(chunk.x - 4, chunk.y - 4, chunk.z - 4);
	glm::ivec3 newMaxBound = glm::ivec3(chunk.x + 4, chunk.y + 4, chunk.z + 4);

	// Remove unneeded backlogged generation packets
	std::vector<ChunkGenerationPacket> newQueue = std::vector<ChunkGenerationPacket>();
	newQueue.reserve(m_ChunksToGenerate.size());
	for (auto p : m_ChunksToGenerate)
	{
		if ((p.Coords.x >= newMinBound.x && p.Coords.x <= newMaxBound.x &&
			p.Coords.y >= newMinBound.y && p.Coords.y <= newMaxBound.y &&
			p.Coords.z >= newMinBound.z && p.Coords.z <= newMaxBound.z))
		{
			newQueue.emplace_back(p);
		}
	}
	m_ChunksToGenerate = newQueue;

	// Unload the outside chunks
	for (int y = oldMinBound.y; y <= oldMaxBound.y; y++)
	{
		for (int z = oldMinBound.z; z <= oldMaxBound.z; z++)
		{
			for (int x = oldMinBound.x; x <= oldMaxBound.x; x++)
			{
				if (!(x >= newMinBound.x && x <= newMaxBound.x &&
					y >= newMinBound.y && y <= newMaxBound.y &&
					z >= newMinBound.z && z <= newMaxBound.z))
				{
					UnloadChunk(x, y, z);
				}
			}
		}
	}

	// Load the unloaded inside chunks
	for (int y = newMinBound.y; y <= newMaxBound.y; y++)
	{
		for (int z = newMinBound.z; z <= newMaxBound.z; z++)
		{
			for (int x = newMinBound.x; x <= newMaxBound.x; x++)
			{
				if (!isLoaded(x, y, z))
				{
					LoadChunk(x, y, z);
				}
			}
		}
	}

	m_CenterChunk = chunk;
}

Chunk* ChunkManager::getChunk(int x, int y, int z) const
{
	try 
	{
		Vec3Hash hash = Vec3Hash(x, y, z);
		return m_Chunks.at(hash).Chunk;
	}
	catch (const std::out_of_range& oor) 
	{
		return nullptr;
	}
}

Mesh* ChunkManager::getMesh(int x, int y, int z) const
{
	try 
	{
		return m_Chunks.at(Vec3Hash(x, y, z)).Mesh;
	}
	catch (const std::out_of_range& oor) 
	{
		return nullptr;
	}
}

bool ChunkManager::isLoaded(int x, int y, int z) const
{
	try
	{
		ChunkPair data = m_Chunks.at(Vec3Hash(x, y, z));
	}
	catch (const std::out_of_range& oor)
	{
		return false;
	}

	return true;
}