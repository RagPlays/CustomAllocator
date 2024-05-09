#include <stdexcept>
#include "FixedSizeAllocator.h"

using namespace dae;

FixedSizeAllocator::FixedSizeAllocator(size_t blockSize, size_t sizeInBytes)
	: m_BlockSize{ blockSize }
	, m_NbBlocks{ sizeInBytes / blockSize }
	, m_Pool{ new Block[m_NbBlocks + 1] }
	, m_Head{ nullptr }
{
}

FixedSizeAllocator::~FixedSizeAllocator()
{
	delete[] m_Pool;
}

void* FixedSizeAllocator::Acquire(const size_t nbBytes)
{
	if (nbBytes > m_BlockSize /*|| no memory left*/) throw std::bad_alloc{};

	/*if (m_Head->next == nullptr)
	{
		throw std::bad_alloc{};
		return nullptr;
	}
	Block* block{ m_Head };
	m_Head = block->next;
	return reinterpret_cast<void*>(block);*/

	return nullptr;
}

void FixedSizeAllocator::Release(void*)
{
	/*Block* block{ reinterpret_cast<Block*>(pointerToBuffer) };

	block->next = m_Head;
	m_Head = block;*/
}