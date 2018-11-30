#pragma once

#ifndef _H_MCNK
#define _H_MCNK

#include <stdint.h>

#include "vectors.h"

struct MCNK
{
	char desc[4];
	uint32_t size;
	uint32_t flags;
	uint32_t index_x;
	uint32_t index_y;
	uint32_t n_layers;
	uint32_t n_doodad_refs;
	uint32_t offset_mcvt;
	uint32_t offset_mcnr;
	uint32_t offset_mcly;
	uint32_t offset_mcrf;
	uint32_t offset_mcal;
	uint32_t size_mcal;
	uint32_t offset_mcsh;
	uint32_t size_mcsh;
	uint32_t areaid;
	uint32_t n_wmo;
	uint16_t holes;
	uint64_t trash0;
	uint64_t trash1;
	uint32_t pred_text;
	uint32_t offset_msce;
	uint32_t n_msce;
	uint32_t offset_mclq;
	uint32_t size_mclq;
	Vector3 position;
	uint32_t offset_mccv;
	uint32_t trash3;
	uint32_t trash4;
};

#endif