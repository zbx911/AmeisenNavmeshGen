#pragma once

#ifndef _H_BLOCK
#define _H_BLOCK

#include "chunk.h"

constexpr double BLOCK_SIZE = 533.33333;

struct Block
{
	Chunk chunks[16 * 16];
};

#endif
