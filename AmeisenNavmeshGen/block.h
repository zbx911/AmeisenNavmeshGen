#pragma once

#ifndef _H_BLOCK
#define _H_BLOCK

#include "chunk.h"

constexpr float BLOCK_SIZE = 533.33333F;

struct Block
{
	Chunk chunks[16 * 16];
};

#endif
