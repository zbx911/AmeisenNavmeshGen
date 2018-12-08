#pragma once

#ifndef _H_MODF
#define _H_MODF

#include <stdint.h>

#include "vectors.h"

enum MODFFlags {
	modf_destroyable = 1,
};

struct MODF_Header {
	char desc[4];
	uint32_t size;
};

struct MODF {
	uint32_t name_id;
	uint32_t unique_id;
	Vector3 position;
	Vector3 rotation;
	AxisAlignedBox extents;
	uint16_t flags;
	uint16_t doodad_set;
	uint16_t name_set;
	uint16_t scale;
};

#endif
