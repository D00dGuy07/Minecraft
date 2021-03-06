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
			glm::vec2(4.0f, 4.0f),
			glm::vec2(4.0f, 4.0f),
			glm::vec2(4.0f, 4.0f),
			glm::vec2(4.0f, 4.0f),
			glm::vec2(4.0f, 4.0f),
			glm::vec2(4.0f, 4.0f),
		}
	});

	// Grass Voxel Data
	BlockData::addBlock({
		1,
		MeshType::CUBE,
		false,
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(2.0f, 0.0f),
		}
	});

	// Dirt Voxel Data
	BlockData::addBlock({
		2,
		MeshType::CUBE,
		false,
		{
			glm::vec2(2.0f, 0.0f),
			glm::vec2(2.0f, 0.0f),
			glm::vec2(2.0f, 0.0f),
			glm::vec2(2.0f, 0.0f),
			glm::vec2(2.0f, 0.0f),
			glm::vec2(2.0f, 0.0f),
		}
	});

	// Cobblestone Voxel Data
	BlockData::addBlock({
		3,
		MeshType::CUBE,
		false,
		{
			glm::vec2(3.0f, 0.0f),
			glm::vec2(3.0f, 0.0f),
			glm::vec2(3.0f, 0.0f),
			glm::vec2(3.0f, 0.0f),
			glm::vec2(3.0f, 0.0f),
			glm::vec2(3.0f, 0.0f),
		}
	});

	// Sand Voxel Data
	BlockData::addBlock({
		4,
		MeshType::CUBE,
		false,
		{
			glm::vec2(5.0f, 0.0f),
			glm::vec2(5.0f, 0.0f),
			glm::vec2(5.0f, 0.0f),
			glm::vec2(5.0f, 0.0f),
			glm::vec2(5.0f, 0.0f),
			glm::vec2(5.0f, 0.0f),
		}
	});

	// Unkown Voxel Data
	BlockData::addBlock({
		5,
		MeshType::CUBE,
		false,
		{
			glm::vec2(4.0f, 0.0f),
			glm::vec2(4.0f, 0.0f),
			glm::vec2(4.0f, 0.0f),
			glm::vec2(4.0f, 0.0f),
			glm::vec2(4.0f, 0.0f),
			glm::vec2(4.0f, 0.0f),
		}
	});
}