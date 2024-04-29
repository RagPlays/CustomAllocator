#include <gtest/gtest.h>
#include <vector>
#include <cstring> // for std::memset

#include "MemoryOverrides.h"
#include "SingleLinkAllocator.h"

#if __GNUC__
// ignoring some gcc warnings because we are deliberately messing with memory
#pragma GCC diagnostic ignored "-Wdelete-incomplete"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#endif

namespace dae
{
	const size_t block_size = sizeof(void*) * 4;
	const size_t allocator_size = 1024;
	const size_t pointer_size = sizeof(void*);

	class Object
	{
	public:
		int m_integer{ 0 };
		float m_float{ 0 };
	};

	TEST(SingleLinkAllocatorTests, SingleAllocation)
	{
		SingleLinkAllocator allocator(allocator_size);

		const size_t test_size = allocator_size - pointer_size;
		void* pointer;
		pointer = allocator.Acquire(test_size);
		EXPECT_NE(pointer, nullptr);
		std::memset(pointer, 1, test_size);
		allocator.Release(pointer);
	}

	TEST(SingleLinkAllocatorTests, NewDelete)
	{
		SingleLinkAllocator allocator(allocator_size);

		Object* pointer = new (allocator) Object();
		EXPECT_NE(pointer, nullptr);
		std::memset(pointer, 1, sizeof(*pointer));
		delete pointer;
	}

	TEST(SingleLinkAllocatorTests, InvalidRelease)
	{
		SingleLinkAllocator allocator(allocator_size);

		void* pointer = new char;
		EXPECT_THROW(allocator.Release(pointer), std::runtime_error);
		delete pointer;
	}

	TEST(SingleLinkAllocatorTests, TwoAllocations)
	{
		SingleLinkAllocator allocator(allocator_size);

		const size_t test_size = allocator_size / 2 - pointer_size;
		void* pointer_a{};
		void* pointer_b{};
		pointer_a = allocator.Acquire(test_size);
		pointer_b = allocator.Acquire(test_size);
		EXPECT_NE(pointer_a, nullptr);
		EXPECT_NE(pointer_b, nullptr);
		std::memset(pointer_a, 1, test_size);
		std::memset(pointer_b, 1, test_size);
		allocator.Release(pointer_a);
		allocator.Release(pointer_b);
	}

	TEST(SingleLinkAllocatorTests, ManySmallAllocations)
	{
		SingleLinkAllocator allocator(allocator_size);

		const size_t nbPieces = allocator_size / block_size;
		void** pointers = new void* [nbPieces];
		const size_t test_size = allocator_size / nbPieces - pointer_size;
		for (unsigned int i = 0; i < nbPieces; i++)
		{
			pointers[i] = allocator.Acquire(test_size);
			EXPECT_NE(pointers[i], nullptr);
			std::memset(pointers[i], 1, test_size);
		}
		for (unsigned int i = 0; i < nbPieces; i++)
			allocator.Release(pointers[i]);
		delete[] pointers;
	}

	TEST(SingleLinkAllocatorTests, OutOfMemory)
	{
		SingleLinkAllocator allocator(allocator_size);
		const size_t test_size = allocator_size - pointer_size;
		void* pointer;
		pointer = allocator.Acquire(test_size);
		EXPECT_NE(pointer, nullptr);
		std::memset(pointer, 1, test_size);
		EXPECT_THROW(allocator.Acquire(4), std::bad_alloc);
		allocator.Release(pointer);
	}

	// Test with caps and allocate double side that will not fit
}


