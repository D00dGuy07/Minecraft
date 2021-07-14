#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct FaceTextures
{
	glm::ivec2 Front;
	glm::ivec2 Right;
	glm::ivec2 Left;
	glm::ivec2 Back;
	glm::ivec2 Top;
	glm::ivec2 Bottom;
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