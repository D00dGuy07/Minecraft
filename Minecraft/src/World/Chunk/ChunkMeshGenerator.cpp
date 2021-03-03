#include "ChunkMeshGenerator.h"

#include "TextureAtlas.h"
#include "BlockData.h"

#include <vector>
#include <array>

#define NO_BLOCK (uint16_t)65535

std::shared_ptr<Mesh> ChunkMeshGenerator::generateChunkMesh(const Chunk& chunk)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	int verticesSize = 0;

	for (int y = 0; y < 16; y++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int x = 0; x < 16; x++)
			{
				SurroundingBlocks neighbors = getSurroundingBlocks(x, y, z, chunk);

				uint16_t block = chunk.getBlock(x, y, z);

				Block data = BlockData::getBlockData(block);

				if (data.Mesh == MeshType::CUBE && (neighbors.Front == NO_BLOCK || BlockData::getBlockData(neighbors.Front).Transparent == true))
				{
					addFace(
						(float)x, (float)y, (float)z,
						FaceType::FRONT,
						vertices,
						indices,
						verticesSize,
						data.Textures.Front.x, data.Textures.Front.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Back == NO_BLOCK || BlockData::getBlockData(neighbors.Back).Transparent == true))
				{
					addFace(
						(float)x, (float)y, (float)z,
						FaceType::BACK,
						vertices,
						indices,
						verticesSize,
						data.Textures.Back.x, data.Textures.Back.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Left == NO_BLOCK || BlockData::getBlockData(neighbors.Left).Transparent == true))
				{
					addFace(
						(float)x, (float)y, (float)z,
						FaceType::LEFT,
						vertices,
						indices,
						verticesSize,
						data.Textures.Left.x, data.Textures.Left.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Right == NO_BLOCK || BlockData::getBlockData(neighbors.Right).Transparent == true))
				{
					addFace(
						(float)x, (float)y, (float)z,
						FaceType::RIGHT,
						vertices,
						indices,
						verticesSize,
						data.Textures.Right.x, data.Textures.Right.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Top == NO_BLOCK || BlockData::getBlockData(neighbors.Top).Transparent == true))
				{
					addFace(
						(float)x, (float)y, (float)z,
						FaceType::TOP,
						vertices,
						indices,
						verticesSize,
						data.Textures.Top.x, data.Textures.Top.y
					);
				}
				if (data.Mesh == MeshType::CUBE && (neighbors.Bottom == NO_BLOCK || BlockData::getBlockData(neighbors.Bottom).Transparent == true))
				{
					addFace(
						(float)x, (float)y, (float)z,
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

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	std::shared_ptr<Mesh> mesh = std::shared_ptr<Mesh>(
		new Mesh()
	);

	mesh->SetVertices(vertices.data(), vertices.size() * sizeof(float));
	mesh->SetIndices(indices.data(), indices.size());

	mesh->BufferLayout.Push<float>(3);
	mesh->BufferLayout.Push<float>(2);
	mesh->BuildVertexArray();

	return mesh;
}

SurroundingBlocks ChunkMeshGenerator::getSurroundingBlocks(const int& x, const int& y, const int& z, const Chunk& chunk)
{
	return {
		(z - 1 >= 0) ? chunk.getBlock(x, y, z - 1) : NO_BLOCK,
		(z + 1 < 16) ? chunk.getBlock(x, y, z + 1) : NO_BLOCK,
		(x - 1 >= 0) ? chunk.getBlock(x - 1, y, z) : NO_BLOCK,
		(x + 1 < 16) ? chunk.getBlock(x + 1, y, z) : NO_BLOCK,
		(y + 1 < 16) ? chunk.getBlock(x, y + 1, z) : NO_BLOCK,
		(y - 1 >= 0) ? chunk.getBlock(x, y - 1, z) : NO_BLOCK
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
		std::array<float, 20> verts = {
			x + -0.5f, y +  0.5f, z + -0.5f, texCoords.v3.x, texCoords.v3.y, // 3
			x +  0.5f, y +  0.5f, z + -0.5f, texCoords.v2.x, texCoords.v2.y, // 2
			x +  0.5f, y + -0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, // 1
			x + -0.5f, y + -0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::BACK:
	{
		std::array<float, 20> verts = {
			x +  0.5f, y +  0.5f, z + 0.5f, texCoords.v3.x, texCoords.v3.y, // 3
			x + -0.5f, y +  0.5f, z + 0.5f, texCoords.v2.x, texCoords.v2.y, // 2
			x + -0.5f, y + -0.5f, z + 0.5f, texCoords.v1.x, texCoords.v1.y, // 1
			x +  0.5f, y + -0.5f, z + 0.5f, texCoords.v0.x, texCoords.v0.y  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::LEFT:
	{
		std::array<float, 20> verts = {
			x + -0.5f, y +  0.5f, z +  0.5f, texCoords.v3.x, texCoords.v3.y, // 3
			x + -0.5f, y +  0.5f, z + -0.5f, texCoords.v2.x, texCoords.v2.y, // 2
			x + -0.5f, y + -0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, // 1
			x + -0.5f, y + -0.5f, z +  0.5f, texCoords.v0.x, texCoords.v0.y  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::RIGHT:
	{
		std::array<float, 20> verts = {
			x +  0.5f, y +  0.5f, z + -0.5f, texCoords.v3.x, texCoords.v3.y, // 3
			x +  0.5f, y +  0.5f, z +  0.5f, texCoords.v2.x, texCoords.v2.y, // 2
			x +  0.5f, y + -0.5f, z +  0.5f, texCoords.v1.x, texCoords.v1.y, // 1
			x +  0.5f, y + -0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::TOP:
	{
		std::array<float, 20> verts = {
			x + -0.5f, y +  0.5f, z +  0.5f, texCoords.v3.x, texCoords.v3.y, // 3
			x +  0.5f, y +  0.5f, z +  0.5f, texCoords.v2.x, texCoords.v2.y, // 2
			x +  0.5f, y +  0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, // 1
			x + -0.5f, y +  0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y  // 0
		};

		vertices.insert(vertices.end(), verts.begin(), verts.end());

		break;
	}
	case FaceType::BOTTOM:
	{
		std::array<float, 20> verts = {
			x +  0.5f, y + -0.5f, z +  0.5f, texCoords.v3.x, texCoords.v3.y, // 3
			x + -0.5f, y + -0.5f, z +  0.5f, texCoords.v2.x, texCoords.v2.y, // 2
			x + -0.5f, y + -0.5f, z + -0.5f, texCoords.v1.x, texCoords.v1.y, // 1
			x +  0.5f, y + -0.5f, z + -0.5f, texCoords.v0.x, texCoords.v0.y  // 0
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

