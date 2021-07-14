#include "DataLoader.h"

#include "BlockData.h"

#include "glm/glm.hpp"

void DataLoader::LoadBlocks()
{
	// Air Voxel Data
	BlockData::addBlock({
		0,
		MeshType::NONE,
		true,
		{
			glm::ivec2(4, 4),
			glm::ivec2(4, 4),
			glm::ivec2(4, 4),
			glm::ivec2(4, 4),
			glm::ivec2(4, 4),
			glm::ivec2(4, 4),
		}
	});

	// Grass Voxel Data
	BlockData::addBlock({
		1,
		MeshType::CUBE,
		false,
		{
			glm::ivec2(0, 0),
			glm::ivec2(0, 0),
			glm::ivec2(0, 0),
			glm::ivec2(0, 0),
			glm::ivec2(1, 0),
			glm::ivec2(2, 0),
		}
	});

	// Dirt Voxel Data
	BlockData::addBlock({
		2,
		MeshType::CUBE,
		false,
		{
			glm::ivec2(2, 0),
			glm::ivec2(2, 0),
			glm::ivec2(2, 0),
			glm::ivec2(2, 0),
			glm::ivec2(2, 0),
			glm::ivec2(2, 0),
		}
	});

	// Cobblestone Voxel Data
	BlockData::addBlock({
		3,
		MeshType::CUBE,
		false,
		{
			glm::ivec2(3, 0),
			glm::ivec2(3, 0),
			glm::ivec2(3, 0),
			glm::ivec2(3, 0),
			glm::ivec2(3, 0),
			glm::ivec2(3, 0),
		}
	});

	// Sand Voxel Data
	BlockData::addBlock({
		4,
		MeshType::CUBE,
		false,
		{
			glm::ivec2(5, 0),
			glm::ivec2(5, 0),
			glm::ivec2(5, 0),
			glm::ivec2(5, 0),
			glm::ivec2(5, 0),
			glm::ivec2(5, 0),
		}
	});

	// Unkown Voxel Data
	BlockData::addBlock({
		5,
		MeshType::CUBE,
		false,
		{
			glm::ivec2(4, 0),
			glm::ivec2(4, 0),
			glm::ivec2(4, 0),
			glm::ivec2(4, 0),
			glm::ivec2(4, 0),
			glm::ivec2(4, 0),
		}
	});
}