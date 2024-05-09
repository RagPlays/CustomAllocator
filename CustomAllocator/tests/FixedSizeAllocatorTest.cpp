#include <gtest/gtest.h>
#include <vector>
#include <cstring> // for std::memset

#include "MemoryOverrides.h"
#include "FixedSizeAllocator.h"

#if __GNUC__
// ignoring some gcc warnings because we are deliberately messing with memory
#pragma GCC diagnostic ignored "-Wdelete-incomplete"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#endif

using namespace dae;

// Tests here