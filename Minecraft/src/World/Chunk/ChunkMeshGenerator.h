#pragma once

#include "Chunk.h"
#include "Mesh.h"

#include <cstdint>

enum class FaceType
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

struct SurroundingBlocks
{
	uint16_t Front;
	uint16_t Back;
	uint16_t Left;
	uint16_t Right;
	uint16_t Top;
	uint16_t Bottom;
};

struct SurroundingChunks
{
	Chunk* Front;
	Chunk* Back;
	Chunk* Left;
	Chunk* Right;
	Chunk* Top;
	Chunk* Bottom;
};

class ChunkMeshGenerator
{
public:
	static bool generateChunkMesh(Mesh& mesh, const Chunk& chunk, SurroundingChunks surroundingChunks, int chunkX, int chunkY, int chunkZ);
private:
	static SurroundingBlocks getSurroundingBlocks(const int& x, const int& y, const int& z, const Chunk& chunk, const SurroundingChunks& surroundingChunks);
	static void addFace(
		float x, float y, float z, 
		FaceType face, 
		std::vector<float>& vertices, 
		std::vector<unsigned int>& indices, 
		int& verticesSize,
		const int& texX, const int& texY
	);
};