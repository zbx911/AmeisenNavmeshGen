#pragma once

#ifndef _H_MH2O
#define _H_MH2O

#include <stdint.h>

#include "keys.h"
#include "vectors.h"

struct MH2O {
	uint32_t offset_instances;
	uint32_t layer_count;
	uint32_t offset_attributes;
} chunks[16 * 16];

struct MH2O_attributes {
	uint64_t fishable;
	uint64_t deep;
};

struct MH2O_instance {
	uint16_t liquid_type;
	uint16_t LVF;
	float min_height_level;
	float max_height_level;
	uint8_t x_offset;
	uint8_t y_offset;
	uint8_t width;
	uint8_t height;
	uint32_t offset_exists_bitmap;
	uint32_t offset_vertex_data;
};

#endif
