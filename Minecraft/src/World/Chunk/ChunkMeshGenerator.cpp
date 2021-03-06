#include "ChunkMeshGenerator.h"

#include "TextureAtlas.h"
#include "BlockData.h"

#include <vector>
#include <array>

#define NO_BLOCK (uint16_t)65535

bool ChunkMeshGenerator::generateChunkMesh(Mesh& mesh, const Chunk& chunk, SurroundingChunks surroundingChunks, int chunkX, int chunkY, int chunkZ)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	int verticesSize = 0;

	bool hasData = false;

	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				SurroundingBlocks neighbors = getSurroundingBlocks(x, y, z, chunk, surroundingChunks);

				uint16_t block = chunk.getBlock(x, y, z);

				Block data = BlockData::getBlockData(block);

				int blockX = x + (chunkX * CHUNK_SIZE);
				int blockY = y + (chunkY * CHUNK_SIZE);
				int blockZ = z + (chunkZ * CHUNK_SIZE);

				if (data.Mesh == MeshType::CUBE && (neighbors.Front == NO_BLOCK || BlockData::getBlockData(neighbors.Front).Transparent == true))
				{
					hasData = true;
					addFace(
						(float)blockX, (float)blockY, (float)blockZ,
						FaceType::FRONT,
						vertices,
						indices,
						verticesSize,
						data.Textures.Front.x, data.Textures.Front.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Back == NO_BLOCK || BlockData::getBlockData(neighbors.Back).Transparent == true))
				{
					hasData = true;
					addFace(
						(float)blockX, (float)blockY, (float)blockZ,
						FaceType::BACK,
						vertices,
						indices,
						verticesSize,
						data.Textures.Back.x, data.Textures.Back.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Left == NO_BLOCK || BlockData::getBlockData(neighbors.Left).Transparent == true))
				{
					hasData = true;
					addFace(
						(float)blockX, (float)blockY, (float)blockZ,
						FaceType::LEFT,
						vertices,
						indices,
						verticesSize,
						data.Textures.Left.x, data.Textures.Left.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Right == NO_BLOCK || BlockData::getBlockData(neighbors.Right).Transparent == true))
				{
					hasData = true;
					addFace(
						(float)blockX, (float)blockY, (float)blockZ,
						FaceType::RIGHT,
						vertices,
						indices,
						verticesSize,
						data.Textures.Right.x, data.Textures.Right.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Top == NO_BLOCK || BlockData::getBlockData(neighbors.Top).Transparent == true))
				{
					hasData = true;
					addFace(
						(float)blockX, (float)blockY, (float)blockZ,
						FaceType::TOP,
						vertices,
						indices,
						verticesSize,
						data.Textures.Top.x, data.Textures.Top.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Bottom == NO_BLOCK || BlockData::getBlockData(neighbors.Bottom).Transparent == true))
				{
					hasData = true;
					addFace(
						(float)blockX, (float)blockY, (float)blockZ,
						FaceType::BOTTOM,
						vertices,
						indices,
						verticesSize,
						data.Textures.Bottom.x, data.Textures.Bottom.y
					);
				}
			}
		}
	}

	if (hasData)
	{
		vertices.shrink_to_fit();
		indices.shrink_to_fit();

		mesh.SetVertices(vertices.data(), vertices.size() * sizeof(float));
		mesh.SetIndices(indices.data(), indices.size());

		mesh.BufferLayout.Push<float>(3);
		mesh.BufferLayout.Push<float>(2);
		mesh.BufferLayout.Push<float>(1);
		mesh.BuildVertexArray();

		return true;
	}
	else
	{
		return false;
	}
	
}

SurroundingBlocks ChunkMeshGenerator::getSurroundingBlocks(const int& x, const int& y, const int& z, const Chunk& chunk, const SurroundingChunks& surroundingChunks)
{
	return {
		(z - 1 >= 0) ? chunk.getBlock(x, y, z - 1) : (surroundingChunks.Front  != nullptr) ? surroundingChunks.Front-> getBlock(x, y, 31) : NO_BLOCK,
		(z + 1 < 32) ? chunk.getBlock(x, y, z + 1) : (surroundingChunks.Back   != nullptr) ? surroundingChunks.Back->  getBlock(x, y,  0) : NO_BLOCK,
		(x - 1 >= 0) ? chunk.getBlock(x - 1, y, z) : (surroundingChunks.Left   != nullptr) ? surroundingChunks.Left->  getBlock(31, y, z) : NO_BLOCK,
		(x + 1 < 32) ? chunk.getBlock(x + 1, y, z) : (surroundingChunks.Right  != nullptr) ? surroundingChunks.Right-> getBlock( 0, y, z) : NO_BLOCK,
		(y + 1 < 32) ? chunk.getBlock(x, y + 1, z) : (surroundingChunks.Top    != nullptr) ? surroundingChunks.Top->   getBlock(x,  0, z) : NO_BLOCK,
		(y - 1 >= 0) ? chunk.getBlock(x, y - 1, z) : (surroundingChunks.Bottom != nullptr) ? surroundingChunks.Bottom->getBlock(x, 31, z) : NO_BLOCK
	};
}

void ChunkMeshGenerator::addFace(
	float x, float y, float z, 
	FaceType face, 
	std::vector<float>& vertices, 
	std::vector<unsigned int>& indices, 
	int& verticesSize,
	const int& texX, const int& texY
)
{
	TextureCoords texCoords = TextureAtlas::GetCoords(texX, texY);

	int startingVerts = vertices.size();

	switch (face)
	{
	case FaceType::FRONT:
	{
		std::array<float, 24> verts = {
			x + -0.5f, y +  0.5f, z + -0.5f, texCoords.v3.x, texCoords.v3.y, 0.86f, // 3
			x +  0.5f, y +  0.5f, z + -0.5f, texCoords.v2.x, texCoords.v2.y, 0.86f, // 2
			x +  0.5f, y + -0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, 0.86f, // 1
			x + -0.5f, y + -0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y, 0.86f  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::BACK:
	{
		std::array<float, 24> verts = {
			x +  0.5f, y +  0.5f, z +  0.5f, texCoords.v3.x, texCoords.v3.y, 0.7f, // 3
			x + -0.5f, y +  0.5f, z +  0.5f, texCoords.v2.x, texCoords.v2.y, 0.7f, // 2
			x + -0.5f, y + -0.5f, z +  0.5f, texCoords.v1.x, texCoords.v1.y, 0.7f, // 1
			x +  0.5f, y + -0.5f, z +  0.5f, texCoords.v0.x, texCoords.v0.y, 0.7f  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::LEFT:
	{
		std::array<float, 24> verts = {
			x + -0.5f, y +  0.5f, z +  0.5f, texCoords.v3.x, texCoords.v3.y, 0.8f, // 3
			x + -0.5f, y +  0.5f, z + -0.5f, texCoords.v2.x, texCoords.v2.y, 0.8f, // 2
			x + -0.5f, y + -0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, 0.8f, // 1
			x + -0.5f, y + -0.5f, z +  0.5f, texCoords.v0.x, texCoords.v0.y, 0.8f // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::RIGHT:
	{
		std::array<float, 24> verts = {
			x +  0.5f, y +  0.5f, z + -0.5f, texCoords.v3.x, texCoords.v3.y, 0.75f, // 3
			x +  0.5f, y +  0.5f, z +  0.5f, texCoords.v2.x, texCoords.v2.y, 0.75f, // 2
			x +  0.5f, y + -0.5f, z +  0.5f, texCoords.v1.x, texCoords.v1.y, 0.75f, // 1
			x +  0.5f, y + -0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y, 0.75f // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::TOP:
	{
		std::array<float, 24> verts = {
			x + -0.5f, y +  0.5f, z +  0.5f, texCoords.v3.x, texCoords.v3.y, 1.0f, // 3
			x +  0.5f, y +  0.5f, z +  0.5f, texCoords.v2.x, texCoords.v2.y, 1.0f, // 2
			x +  0.5f, y +  0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, 1.0f, // 1
			x + -0.5f, y +  0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y, 1.0f  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::BOTTOM:
	{
		std::array<float, 24> verts = {
			x +  0.5f, y + -0.5f, z +  0.5f, texCoords.v3.x, texCoords.v3.y, 0.67f, // 3
			x + -0.5f, y + -0.5f, z +  0.5f, texCoords.v2.x, texCoords.v2.y, 0.67f, // 2
			x + -0.5f, y + -0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, 0.67f, // 1
			x +  0.5f, y + -0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y, 0.67f  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	}

	std::array<unsigned int, 6> _indices = {
			verticesSize + 0, verticesSize + 1, verticesSize + 2,
			verticesSize + 2, verticesSize + 3, verticesSize + 0
	};

	verticesSize += 4;

	indices.insert(indices.end(), _indices.begin(), _indices.end());
}

