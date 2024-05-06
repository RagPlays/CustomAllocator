#include <cstdint>
#include <stdexcept>
#include "StackAllocator.h"

using namespace dae;

StackAllocator::StackAllocator(const size_t sizeInBytes)
	: m_Size{ sizeInBytes }
	, m_Pool{ new uint8_t[sizeInBytes] } // use new keyword to allocate the memory
{
	m_StackPointer = m_Pool;
}

StackAllocator::~StackAllocator()
{
	delete[] m_Pool;
}

void* StackAllocator::Acquire(const size_t nbBytes)
{
	if(static_cast<uint8_t*>(m_StackPointer) + nbBytes > static_cast<uint8_t*>(m_Pool) + m_Size)
	{
		throw std::bad_alloc{};
	}
	void* marker = m_StackPointer;
	m_StackPointer = static_cast<uint8_t*>(m_StackPointer) + nbBytes;
	return marker;
}

void StackAllocator::Release(void* marker)
{
	// check if pointer is within the pool
	if (marker >= m_Pool && marker < m_StackPointer)
	{
		m_StackPointer = marker;
	}
	else
	{
		throw std::runtime_error{ "Invalid marker" };
	}
}

void StackAllocator::ReleaseAll()
{
	m_StackPointer = m_Pool;
}

void* StackAllocator::GetMarker() const
{
	return m_StackPointer;
}