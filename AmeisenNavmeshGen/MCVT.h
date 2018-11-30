#pragma once

#ifndef _H_MCVT
#define _H_MCVT

#include <stdint.h>

struct MCVT
{
	char desc[4];
	uint32_t size;
	char trash;
	float height[9 * 9 + 8 * 8];
};

#endif