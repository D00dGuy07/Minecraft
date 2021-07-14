#pragma once

#include "Chunk.h"
#include "Mesh.h"

#include "glm/glm.hpp"

#include "TerrainGenerator.h"
#include "ChunkMeshGenerator.h"

#include "Vec3Hash.h"

#include <vector>
#include <unordered_map>
#include <iostream>

struct ChunkGenerationPacket
{
	glm::ivec3 Coords;
	bool GenerateBlocks;
	bool BuildMesh;
	bool BuildNeighbors;
};

struct ChunkPair
{
	Chunk* Chunk;
	Mesh* Mesh;
};

class ChunkManager
{
public:
	ChunkManager(int x, int y, int z);
	~ChunkManager();
	
	void GenerateChunk();

	void LoadChunk(int x, int y, int z);
	void UnloadChunk(int x, int y, int z);

	void SetMiddleChunk(glm::ivec3 chunk);

	glm::vec3 GetCenterPos() { return m_CenterChunk; }

	Chunk* getChunk(int x, int y, int z) const;
	Mesh* getMesh(int x, int y, int z) const;
private:
	SurroundingChunks getNeighboringChunks(int x, int y, int z) {
		return {
			getChunk(x,     y,     z + 1),
			getChunk(x,     y,     z - 1),
			getChunk(x - 1, y,     z),
			getChunk(x + 1, y,     z),
			getChunk(x,     y + 1, z),
			getChunk(x,     y - 1, z)
		};
	}

	void GenerateBlocks(Chunk* chunk, int x, int y, int z);
	bool GenerateMesh(int x, int y, int z);

	bool isLoaded(int x, int y, int z) const;
	
	TerrainGenerator m_TerrainGenerator;

	glm::ivec3 m_CenterChunk;

	std::unordered_map<Vec3Hash, ChunkPair> m_Chunks;
	std::vector<ChunkGenerationPacket> m_ChunksToGenerate;
};