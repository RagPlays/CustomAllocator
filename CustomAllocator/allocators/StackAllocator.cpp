#include "StackAllocator.h"

void* dae::StackAllocator::Acquire(size_t)
{
	return nullptr;
}

void dae::StackAllocator::Release(void*)
{
}
