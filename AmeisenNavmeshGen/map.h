#pragma once

#ifndef _H_MAP
#define _H_MAP

#include "block.h"

constexpr float MAP_SIZE = 34133.33312F;

struct Map
{
	std::string map_name;
	Block blocks[64 * 64];
};

#endif
