#include <cstdint>
#include <stdexcept>
#include "StackAllocator.h"

using namespace dae;

StackAllocator::StackAllocator(const size_t sizeInBytes)
	: m_Size{ sizeInBytes }
	, m_Pool{ new uint8_t[sizeInBytes] } // use new keyword to allocate the memory
{
	Reset();
}

StackAllocator::~StackAllocator()
{
	delete[] m_Pool;
}

void* StackAllocator::GetMarker() const
{
	return m_StackPointer;
}

void StackAllocator::FreeToMarker(uint8_t* marker)
{
	if (marker >= m_Pool && marker < m_StackPointer)
	{
		m_StackPointer = marker;
		return;
	}
	throw std::runtime_error{ "Invalid marker" };
}

void* StackAllocator::Acquire(size_t nbBytes)
{
	if(m_StackPointer + nbBytes > m_Pool + m_Size)
	{
		throw std::bad_alloc{};
	}
	uint8_t* marker = m_StackPointer;
	m_StackPointer = m_StackPointer + nbBytes;
	return marker;
}

void StackAllocator::Release(void*)
{
}

void StackAllocator::Reset()
{
	m_StackPointer = m_Pool;
}