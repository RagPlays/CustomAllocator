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

using namespace dae;

static constexpr size_t BlockSize = sizeof(void*) * 4;
static constexpr size_t AllocatorSize = 1024;
static constexpr size_t PointerSize = sizeof(void*);

class Object
{
public:
	int m_integer{ 0 };
	float m_float{ 0 };
};

TEST(SingleLinkAllocatorTests, SingleAllocation)
{
	SingleLinkAllocator allocator(AllocatorSize);

	const size_t test_size = AllocatorSize - PointerSize;
	void* pointer;
	pointer = allocator.Acquire(test_size);
	EXPECT_NE(pointer, nullptr);
	std::memset(pointer, 1, test_size);
	allocator.Release(pointer);
}

TEST(SingleLinkAllocatorTests, NewDelete)
{
	SingleLinkAllocator allocator(AllocatorSize);

	Object* pointer = new (allocator) Object();
	EXPECT_NE(pointer, nullptr);
	std::memset(pointer, 1, sizeof(*pointer));
	delete pointer;
}

TEST(SingleLinkAllocatorTests, InvalidRelease)
{
	SingleLinkAllocator allocator(AllocatorSize);

	void* pointer = new char;
	EXPECT_THROW(allocator.Release(pointer), std::runtime_error);
	delete pointer;
}

TEST(SingleLinkAllocatorTests, TwoAllocations)
{
	SingleLinkAllocator allocator(AllocatorSize);

	const size_t test_size = AllocatorSize / 2 - PointerSize;
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
	SingleLinkAllocator allocator(AllocatorSize);

	const size_t nbPieces = AllocatorSize / BlockSize;
	void** pointers = new void* [nbPieces];
	const size_t test_size = AllocatorSize / nbPieces - PointerSize;
	for (unsigned int i = 0; i < nbPieces; i++)
	{
		pointers[i] = allocator.Acquire(test_size);
		EXPECT_NE(pointers[i], nullptr);
		std::memset(pointers[i], 1, test_size);
	}
	for (unsigned int i = 0; i < nbPieces; ++i)
	{
		allocator.Release(pointers[i]);
	}
	delete[] pointers;
}

TEST(SingleLinkAllocatorTests, OutOfMemory)
{
	SingleLinkAllocator allocator(AllocatorSize);
	const size_t test_size = AllocatorSize - PointerSize;
	void* pointer;
	pointer = allocator.Acquire(test_size);
	EXPECT_NE(pointer, nullptr);
	std::memset(pointer, 1, test_size);
	EXPECT_THROW(allocator.Acquire(4), std::bad_alloc);
	allocator.Release(pointer);
}

TEST(SingleLinkAllocatorTests, Fragmentation)
{
	SingleLinkAllocator allocator(AllocatorSize);

	const size_t nbPieces = AllocatorSize / BlockSize;
	void** pointers = new void* [nbPieces];
	const size_t test_size = AllocatorSize / nbPieces - PointerSize;

	for (size_t idx{ 0 }; idx < nbPieces; ++idx)
	{
		pointers[idx] = allocator.Acquire(test_size);
		EXPECT_NE(pointers[idx], nullptr);
		std::memset(pointers[idx], 1, test_size);
	}
	for (size_t idx{ 0 }; idx < nbPieces; idx += 2)
	{
		allocator.Release(pointers[idx]);
	}

	EXPECT_THROW(allocator.Acquire(BlockSize * 2), std::bad_alloc);

	delete[] pointers;
}