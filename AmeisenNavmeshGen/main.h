#pragma once

#ifndef _H_MAIN
#define _H_MAIN

#include "common.h"
#include "MVER.h"
#include "MHDR.h"
#include "MCNK.h"
#include "MCVT.h"
#include "MCNR.h"

void read_MVER(MVER &adt_mver, std::ifstream &adt_stream);
void read_MHDR(MHDR &adt_mhdr, std::ifstream &adt_stream);
void read_MCIN_header(MCIN &adt_mcin, std::ifstream &adt_stream);
void read_MCIN_chunks(std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);
void read_MCNK(std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);
void read_MCVT(std::vector<MCVT> &adt_mcvt, std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);
void read_MCNR(std::vector<MCNR> &adt_mcvt, std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);

#endif