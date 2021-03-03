#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct FaceTextures
{
	glm::vec2 Front;
	glm::vec2 Right;
	glm::vec2 Left;
	glm::vec2 Back;
	glm::vec2 Top;
	glm::vec2 Bottom;
};

enum class MeshType {
	NONE,
	CUBE
};

struct Block
{
public:
	unsigned short BlockID;
	MeshType Mesh;
	bool Transparent;

	FaceTextures Textures;
};

class BlockData
{
private:
	static std::unordered_map<unsigned short, Block> m_BlockArchive;

public:
	static Block getBlockData(unsigned short blockID);

	static void addBlock(Block block);
};