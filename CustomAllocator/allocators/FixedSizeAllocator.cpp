#include <stdexcept>
#include <cstring>
#include "FixedSizeAllocator.h"

static const unsigned char PATTERN_ALIGN = 0xFC;
static const unsigned char PATTERN_ALLOC = 0x00;

dae::FixedSizeAllocator::FixedSizeAllocator(size_t blockSize, size_t sizeInBytes)
	: m_BlockSize{ blockSize }
	, nbBlocks{ sizeInBytes / blockSize }
	, m_Pool{ new Block[nbBlocks + 1] }
	, m_Head{m_Pool}
{
	Block* first = m_Pool + 1;
	first->next = nullptr;

	m_Head->next = first;

	for (size_t i = 0; i < nbBlocks - 1; ++i)
	{
		Block* block = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(m_Pool) + i * blockSize);
		block->next = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(m_Pool) + (i + 1) * blockSize);
	}

	Block* lastBlock = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(m_Pool) + (nbBlocks - 1) * blockSize);
	lastBlock->next = nullptr;
}

void* dae::FixedSizeAllocator::Acquire(size_t)
{
	return nullptr;
}

void dae::FixedSizeAllocator::Release(void*)
{
}