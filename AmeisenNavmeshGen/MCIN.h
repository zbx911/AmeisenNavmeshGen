#pragma once

#ifndef _H_MCIN
#define _H_MCIN

#include <stdint.h>

struct MCIN_chunk
{
	uint32_t offset;
	uint32_t size;
	uint32_t flags;
	union
	{
		char pad[4];
		uint32_t asyncId;
	};
};

struct MCIN
{
	char desc[4];
	uint32_t size;
};

#endif
