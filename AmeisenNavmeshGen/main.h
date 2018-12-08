#pragma once

#ifndef _H_MAIN
#define _H_MAIN

#include "common.h"
#include "MVER.h"
#include "MHDR.h"
#include "MCNK.h"
#include "MCVT.h"
#include "MCNR.h"
#include "MWMO.h"
#include "MODF.h"

void read_MVER(MVER &adt_mver, std::ifstream &adt_stream);
void read_MHDR(MHDR &adt_mhdr, std::ifstream &adt_stream);

void read_MCIN_header(MCIN &adt_mcin, std::ifstream &adt_stream);
void read_MCIN_chunks(std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);
void read_MCNK(std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);
void read_MCVT(std::vector<MCVT> &adt_mcvt, std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);
void read_MCNR(std::vector<MCNR> &adt_mcvt, std::vector<MCNK> &adt_mcnks, std::vector<MCIN_chunk> &adt_mcin_chunks, std::ifstream &adt_stream);

void read_MWMO(MWMO &adt_mwmo, std::vector<std::string> &wmo_paths, std::ifstream &adt_stream);
void read_MODF(MODF_Header &adt_modf_header, std::vector<MODF> &adt_modfs, std::ifstream &adt_stream);
void read_MWID(MWID &adt_mwid, std::vector<uint32_t> &adt_mwids, std::ifstream &adt_stream);

void read_MMDX(MMDX &adt_mmdx, std::vector<std::string> &m2_paths, std::ifstream &adt_stream);
void read_MDDF(MDDF_Header &adt_mddf_header, std::vector<MDDF> &adt_mddfs, std::ifstream &adt_stream);
void read_MMID(MMID &adt_mmid, std::vector<uint32_t> &adt_mmids, std::ifstream &adt_stream);

void output_parsing_debug_stuff(MVER adt_mver, MHDR adt_mhdr, MCIN adt_mcin, std::vector<MCIN_chunk> adt_mcin_chunks, std::vector<MCNK> adt_mcnks, std::vector<MCVT> adt_mcvts, std::vector<MCNR> adt_mcnrs);
#endif
