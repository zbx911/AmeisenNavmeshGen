#pragma once

#ifndef _H_MVER
#define _H_MVER

#include <stdint.h>

struct MVER {
	char desc[4];
	uint32_t size;
	uint32_t version;
};

#endif
