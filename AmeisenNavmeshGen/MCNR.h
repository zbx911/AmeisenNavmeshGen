#pragma once

#ifndef _H_MCNR
#define _H_MCNR

#include <stdint.h>

struct MCNR
{
	char desc[4];
	uint32_t size;
	float normals[9 * 9 + 8 * 8];
};

#endif