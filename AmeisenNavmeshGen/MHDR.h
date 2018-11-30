#pragma once

#ifndef _H_MHDR
#define _H_MHDR

#include <stdint.h>

#include "MCIN.h"
#include "MTEX.h"
#include "MMDX.h"
#include "MMDX.h"
#include "MMID.h"
#include "MWMO.h"
#include "MWID.h"
#include "MDDF.h"
#include "MODF.h"
#include "MFBO.h"
#include "MH2O.h"
#include "MTXF.h"

enum MHDRFlags {
	mhdr_MFBO = 1,
	mhdr_northrend = 2,
};

struct MHDR {
	char desc[4];
	uint32_t size;
	uint32_t flags;
	MCIN* mcin;
	MTEX* mtex;
	MMDX* mmdx;
	MMID* mmid;
	MWMO* mwmo;
	MWID* mwid;
	MDDF* mddf;
	MODF* modf;
	MFBO* mfbo;
	MH2O* mh2o;
	MTXF* mtxf;
	uint32_t unused[4];
};

#endif
