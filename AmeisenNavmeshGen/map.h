#pragma once

#ifndef _H_MAP
#define _H_MAP

#include "block.h"

constexpr double MAP_SIZE = 34133.33312;

struct Map
{
	std::string map_name;
	Block blocks[64 * 64];
};

#endif
