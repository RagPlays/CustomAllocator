#ifndef FIXEDSIZEALLOCATOR_H
#define FIXEDSIZEALLOCATOR_H

#include "MemoryAllocator.h"

namespace dae
{
	class FixedSizeAllocator final : public MemoryAllocator
	{
	public:

		explicit FixedSizeAllocator(size_t blockSize, size_t sizeInBytes);
		virtual ~FixedSizeAllocator();

		FixedSizeAllocator(const FixedSizeAllocator& other) = delete;
		FixedSizeAllocator(FixedSizeAllocator&& other) noexcept = delete;
		FixedSizeAllocator& operator=(const FixedSizeAllocator& other) = delete;
		FixedSizeAllocator& operator=(FixedSizeAllocator&& other) noexcept = delete;

		virtual void* Acquire(const size_t nbBytes) override;
		virtual void Release(void* pointerToBuffer) override;

	private:

		struct Block
		{
			// size of block (blocksize) "data"
			Block* next;
		};
		const size_t m_BlockSize;
		size_t m_NbBlocks;
		Block* m_Pool;
		Block* m_Head;

	};
}

#endif // !FIXEDSIZEALLOCATOR_H