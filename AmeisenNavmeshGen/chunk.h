#pragma once

#ifndef _H_CHUNK
#define _H_CHUNK

#include <vector>
#include <stdint.h>

#include "vectors.h"

constexpr auto CHUNK_SIZE = 33.3333;

struct Chunk
{
	float heights[9 * 9 + 8 * 8];
	float normals[9 * 9 + 8 * 8];

	Vector3 position;

	std::vector<Vector3> vertices;
	std::vector<uint32_t> vertices;
};

#endif
