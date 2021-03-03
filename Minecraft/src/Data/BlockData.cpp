#include "BlockData.h"

std::unordered_map<unsigned short, Block> BlockData::m_BlockArchive;

Block BlockData::getBlockData(unsigned short blockID)
{
	return m_BlockArchive[blockID];
}

void BlockData::addBlock(Block block)
{
	m_BlockArchive[block.BlockID] = block;
}
