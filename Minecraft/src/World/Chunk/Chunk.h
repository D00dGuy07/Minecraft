#pragma once

#include <cstdint>

struct Chunk
{
	uint16_t blocks[4096] = {1};

	uint16_t getBlock(int x, int y, int z) const { return blocks[x + (z << 4) + (y << 8)]; }
	void setBlock(uint16_t block, int x, int y, int z) { blocks[x + (z << 4) + (y << 8)] = block; }
};