#pragma once

#include <cstdint>

#define CHUNK_SIZE 32

struct Chunk
{
	uint16_t blocks[32768];

	uint16_t getBlock(int x, int y, int z) const { return blocks[x + (z << 5) + (y << 10)]; }
	void setBlock(uint16_t block, int x, int y, int z) { blocks[x + (z << 5) + (y << 10)] = block; }
};