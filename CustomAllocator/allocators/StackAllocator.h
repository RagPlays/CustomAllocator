#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

#include "MemoryAllocator.h"

namespace dae
{
	class StackAllocator final : public MemoryAllocator
	{
	public:

		explicit StackAllocator(const size_t sizeInBytes);
		virtual ~StackAllocator();

		StackAllocator(const StackAllocator& other) = delete;
		StackAllocator(StackAllocator&& other) noexcept = delete;
		StackAllocator& operator=(const StackAllocator& other) = delete;
		StackAllocator& operator=(StackAllocator&& other) noexcept = delete;

		virtual void* Acquire(const size_t nbBytes) override;
		virtual void Release(void* marker) override;
		void ReleaseAll();
		void* GetMarker() const;

	private:

		const size_t m_Size;
		void* m_Pool; // start pointer of stack
		void* m_StackPointer; // current stack pointer

	};
}

#endif // !STACKALLOCATOR_H