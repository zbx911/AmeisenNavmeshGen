#pragma once

#ifndef _H_MDDF
#define _H_MDDF

#include <stdint.h>

#include "vectors.h"

enum MDDFFlags {
	mddf_biodome = 1,
	mddf_shrubbery = 2
};

struct MDDF_Header {
	char desc[4];
	uint32_t size;
};

struct MDDF {
	uint32_t name_id;
	uint32_t unique_id;
	Vector3 position;
	Vector3 rotation;
	uint16_t scale;
	uint16_t flags;
};

#endif
