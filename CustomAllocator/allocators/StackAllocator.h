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

		void* GetMarker() const;
		void FreeToMarker(uint8_t* marker);
		virtual void* Acquire(size_t nbBytes) override;
		virtual void Release(void*) override;
		void Reset();

	private:

		const size_t m_Size;
		uint8_t* m_Pool; // start pointer of stack
		uint8_t* m_StackPointer; // current stack pointer

	};
}

#endif // !STACKALLOCATOR_H