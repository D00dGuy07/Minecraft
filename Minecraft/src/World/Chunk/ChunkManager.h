#pragma once

#include "Chunk.h"
#include "Mesh.h"

#include "glm/glm.hpp"

#include "TerrainGenerator.h"

#include <vector>

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();
	
	void GenerateMeshes();
	void GenerateTerrain();

	std::vector<Mesh*> GetMeshes() { return m_Meshes; }
	bool NeedsGenerating() const { return m_NeedsGenerating; }

private:
	Chunk* getChunk(int x, int y, int z) const { return m_Chunks[x + (z << 3) + (y << 6)]; }
	Mesh* getMesh(int x, int y, int z) const { return m_Meshes[x + (z << 3) + (y << 6)]; }

	TerrainGenerator m_TerrainGenerator;

	glm::vec3 m_CurrentChunk;
	bool m_NeedsGenerating;

	std::vector<Chunk*> m_Chunks;
	std::vector<Mesh*> m_Meshes;
};