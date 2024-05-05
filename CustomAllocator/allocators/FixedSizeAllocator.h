#ifndef FIXEDSIZEALLOCATOR_H
#define FIXEDSIZEALLOCATOR_H

#include "MemoryAllocator.h"

namespace dae
{
	class FixedSizeAllocator final : public MemoryAllocator
	{
	public:

		explicit FixedSizeAllocator(size_t blockSize, size_t sizeInBytes);
		virtual ~FixedSizeAllocator() = default;

		FixedSizeAllocator(const FixedSizeAllocator& other) = delete;
		FixedSizeAllocator(FixedSizeAllocator&& other) noexcept = delete;
		FixedSizeAllocator& operator=(const FixedSizeAllocator& other) = delete;
		FixedSizeAllocator& operator=(FixedSizeAllocator&& other) noexcept = delete;

		virtual void* Acquire(size_t) override;
		virtual void Release(void*) override;

	private:

		struct Block
		{
			Block* next;
		};
		const size_t m_BlockSize;
		size_t nbBlocks;
		Block* m_Pool;
		Block* m_Head;

	};
}

#endif // !FIXEDSIZEALLOCATOR_H