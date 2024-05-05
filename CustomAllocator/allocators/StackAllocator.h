#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

#include "MemoryAllocator.h"

namespace dae
{
	class StackAllocator final : public MemoryAllocator
	{
	public:

		explicit StackAllocator() = default;
		virtual ~StackAllocator() = default;

		StackAllocator(const StackAllocator& other) = delete;
		StackAllocator(StackAllocator&& other) noexcept = delete;
		StackAllocator& operator=(const StackAllocator& other) = delete;
		StackAllocator& operator=(StackAllocator&& other) noexcept = delete;

		virtual void* Acquire(size_t) override;
		virtual void Release(void*) override;

	private:

	};
}

#endif // !STACKALLOCATOR_H