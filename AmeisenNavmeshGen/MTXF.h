#pragma once

#ifndef _H_MTXF
#define _H_MTXF

#include <stdint.h>

struct MTXF
{
	uint32_t trash : 1;
	uint32_t : 3;

	uint32_t : 28;
};

#endif