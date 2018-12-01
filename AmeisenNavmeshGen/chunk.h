#pragma once

#ifndef _H_CHUNK
#define _H_CHUNK

#include <vector>
#include <stdint.h>

#include "vectors.h"

constexpr float CHUNK_SIZE = 33.3333F;
constexpr float TILE_STEP9 = CHUNK_SIZE / 9.F;
constexpr float TILE_STEP8 = CHUNK_SIZE / 8.F;

struct Chunk
{
	Vector3 position;

	float heights[9 * 9 + 8 * 8];
	float normals[9 * 9 + 8 * 8];
};

#endif
